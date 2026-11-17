#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#if 1
typedef vertCache_t * VKActuallyLoadBuffer_t;
#else
typedef struct VKActuallyLoadBuffer_s
{
	vertCache_t *block;

	VKActuallyLoadBuffer_s( vertCache_t *block )
			: block( block )
	{}
	VKActuallyLoadBuffer_s()
			: block( NULL )
	{}
} VKActuallyLoadBuffer_t;
#endif

static idList<VKActuallyLoadBuffer_t> loadBuffers[NUM_INFLIGHT_FRAME];

// only call in frontend in multi-threading
void R_VK_AddLoadBufferList( vertCache_t *block )
{
    loadBuffers[vertexCache.GetListNum()].Append( VKActuallyLoadBuffer_t( block ) );
	//printf("aaa %d %d %p\n", vk_backend.frameCount, vertexCache.GetListNum(), block);
}

// only call in backend after vulkan context command recording in multi-threading
void R_VK_HandleLoadBufferList( void )
{
	idList<VKActuallyLoadBuffer_t> &loadList = loadBuffers[
#ifdef _MULTITHREAD // using listNum when multi-threading is active, else using current frame of vertexCache
		multithreadActive ? renderThread->vertListToRender : vertexCache.GetListNum()
#else
			vertexCache.GetListNum()
#endif
	];
	if (loadList.Num() > 0)
	{
		for(int i = 0; i < loadList.Num(); i++)
		{
			vertexCache.TransferData_VK(loadList[i]);
			//printf("ttt %d %d %p\n", vk_backend.frameCount, multithreadActive ? renderThread->vertListToRender : vertexCache.GetListNum(), loadList[i]);
		}
		loadList.SetNum(0);
	}
}

// current buffer binding using vk_context's frameIndex, so not support access in multi-threading
void idVertexCache::BindBuffer_VK(VkBuffer buffer)
{
	VKASSERT_THREAD;
	currentBoundVBO[vk_context.frameIndex] = buffer;
}

void idVertexCache::BindIndexBuffer_VK(VkBuffer buffer)
{
	VKASSERT_THREAD;
	currentBoundIBO[vk_context.frameIndex] = buffer;
}

/*
==============
idVertexCache::ActuallyFree_VK
==============
*/
void idVertexCache::ActuallyFree_VK(vertCache_t *block)
{
	if (!block) {
		common->Error("idVertexCache VK Free: NULL pointer");
	}

	if (block->user) {
		// let the owner know we have purged it
		*block->user = NULL;
		block->user = NULL;
	}

	// temp blocks are in a shared space that won't be freed
	if (block->tag != TAG_TEMP) {
		staticAllocTotal -= block->size;
		staticCountTotal--;

		if (block->vk_buffer) {
			VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "free_buffer_%d::%d::%s_%s", vk_context.frameIndex, block->vbo, "static", block->indexBuffer ? "index" : "vertex");
			R_VK_DestroyBuffer(block->vbo);
			block->vk_buffer = NULL;
			block->vbo = VKHANDLE_INVALID;
		} else if (block->virtMem) {
			Mem_Free(block->virtMem);
			block->virtMem = NULL;
		}
	}

	block->tag = TAG_FREE;		// mark as free
#ifdef _MULTITHREAD
	if(multithreadActive)
	{
		if(block->virtMem)
		{
			Mem_Free(block->virtMem);
			block->virtMem = NULL;
		}
		block->virtMemDirty = true;
	}
#endif

	// unlink stick it back on the free list
	block->next->prev = block->prev;
	block->prev->next = block->next;

#if 1
	// stick it on the front of the free list so it will be reused immediately
    vertCache_t &freeStaticHeaders = freeStaticHeaderss[VBO_INDEX(block->indexBuffer)];
	block->next = freeStaticHeaders.next;
	block->prev = &freeStaticHeaders;
#else
	// stick it on the back of the free list so it won't be reused soon (just for debugging)
	block->next = &freeStaticHeaders;
	block->prev = freeStaticHeaders.prev;
#endif

	block->next->prev = block;
	block->prev->next = block;
}

/*
===========
idVertexCache::Alloc_VK
===========
*/
void idVertexCache::Alloc_VK(void *data, int size, vertCache_t **buffer, bool indexBuffer)
{
	vertCache_t	*block;

	if (size <= 0) {
		common->Error("idVertexCache::Alloc_VK: size = %i\n", size);
	}

	if(Sys_InRenderThread())
	assert(!vk_context.inCommandRecording || !vk_context.inRenderPass);
	// if we can't find anything, it will be NULL
	*buffer = NULL;

	// if we don't have any remaining unused headers, allocate some more
    const int vboTypeIndex = VBO_INDEX(indexBuffer);
	vertCache_t &freeStaticHeaders = freeStaticHeaderss[vboTypeIndex];
	vertCache_t &staticHeaders = staticHeaderss[vboTypeIndex];
	if (freeStaticHeaders.next == &freeStaticHeaders) {

		for (int i = 0; i < EXPAND_HEADERS; i++) {
			block = headerAllocator.Alloc();
			block->next = freeStaticHeaders.next;
			block->prev = &freeStaticHeaders;
			block->next->prev = block;
			block->prev->next = block;

#ifdef _MULTITHREAD
			if(multithreadActive)
			{
				block->virtMem = NULL;
				block->virtMemDirty = false;
				block->vbo = VKHANDLE_INVALID;
				block->vk_buffer = NULL;
			}
			else
#endif
			block->vbo = VK_AllocBuffer(&block->vk_buffer);
		}
	}

	// move it from the freeStaticHeaders list to the staticHeaders list
	block = freeStaticHeaders.next;
	block->next->prev = block->prev;
	block->prev->next = block->next;
	block->next = staticHeaders.next;
	block->prev = &staticHeaders;
	block->next->prev = block;
	block->prev->next = block;

	block->size = size;
	block->offset = 0;
	block->tag = TAG_USED;

	// save data for debugging
	staticAllocThisFrame += block->size;
	staticCountThisFrame++;
	staticCountTotal++;
	staticAllocTotal += block->size;

	// this will be set to zero when it is purged
	block->user = buffer;
	*buffer = block;

	// allocation doesn't imply used-for-drawing, because at level
	// load time lots of things may be created, but they aren't
	// referenced by the GPU yet, and can be purged if needed.
	block->frameUsed = currentFrame - NUM_VERTEX_FRAMES;

	block->indexBuffer = indexBuffer;

#ifdef _MULTITHREAD
	if(multithreadActive)
	{
		if( block->virtMem )
			Mem_Free(block->virtMem);
		block->virtMem = Mem_Alloc(size);
		SIMDProcessor->Memcpy(block->virtMem, data, size);
		block->virtMemDirty = true;

		R_VK_AddLoadBufferList(block); // will transfer data before render pass beginning, not in Position() called
	}
    else
    {
#endif
    	// copy the data
    	if (block->vk_buffer && block->vk_buffer->buffer)
    	{
    		if (block->vk_buffer->size < block->size)
    		{
				VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "free_buffer_%d::%d::%s_%s", vk_context.frameIndex, block->vbo, "static", block->indexBuffer ? "index" : "vertex");
    			R_VK_DestroyBuffer(block->vbo);
    			block->vk_buffer = NULL;
    			block->vbo = VKHANDLE_INVALID;
    		}
    	}
    	if (!block->vk_buffer)
    	{
    		block->vbo = VK_AllocBuffer(&block->vk_buffer);
    	}
    	if (VK_NULL_HANDLE == block->vk_buffer->buffer)
    	{
    		if (indexBuffer) {
    			VK_CreateIndexBuffer(block->vk_buffer, block->size, false, false); // GPU only
    		} else {
    			VK_CreateVertexBuffer(block->vk_buffer, block->size, false, false); // GPU only
    		}
			VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "buffer::%d::%s_%s", block->vbo, "static", indexBuffer ? "index" : "vertex");
    	}
    	if (data)
    		R_VK_UploadBufferData(block->vk_buffer, size, data, 0);
    	if (indexBuffer) {
    		BindIndexBuffer_VK(block->vk_buffer->buffer);
    	} else {
    		BindBuffer_VK(block->vk_buffer->buffer);
    	}
		block->vk_offset = 0;
#ifdef _MULTITHREAD
    }
#endif
}

/*
==============
idVertexCache::Position_VK

this will be a real pointer with virtual memory,
but it will be an int offset cast to a pointer with
vertex buffer object

The vertex buffer object will be bound
==============
*/
void *idVertexCache::Position_VK(vertCache_t *buffer)
{
	if (!buffer || buffer->tag == TAG_FREE) {
		common->FatalError("idVertexCache::Position: bad vertCache_t");
	}

	// the vertex object just uses an offset
	if (buffer->vk_buffer) {
		if (r_showVertexCache.GetInteger() == 2) {
			if (buffer->tag == TAG_TEMP) {
				common->Printf("GL_ARRAY_BUFFER = %i + %zd (%i bytes)\n", buffer->vbo, buffer->offset, buffer->size);
			} else {
				common->Printf("GL_ARRAY_BUFFER = %i (%i bytes)\n", buffer->vbo, buffer->size);
			}
		}

		if (buffer->indexBuffer) {
			BindIndexBuffer_VK(buffer->vk_buffer->buffer);
		} else {
			BindBuffer_VK(buffer->vk_buffer->buffer);
		}

		return (void *)buffer->vk_offset;
	}
	// Vulkan only in multi-threading: upload static data not in Position() called in render thread, because buffer data must uploaded before render pass beginning

	common->Error("Vulkan must using %s buffer\n", buffer->indexBuffer ? "index" : "vertex");
	// virtual memory is a real pointer
	return (void *)((byte *)buffer->virtMem + buffer->offset);
}

vertCache_t* idVertexCache::CreateTempVbo_VK(int bytes, bool indexBuffer)
{
	vertCache_t* block = headerAllocator.Alloc();

	block->next = NULL;
	block->prev = NULL;
	block->virtMem = NULL;
	block->offset = 0;
	block->tag = TAG_FIXED;
	block->indexBuffer = indexBuffer;
#ifdef _MULTITHREAD
	block->virtMemDirty = false;
#endif

	block->size = bytes;
	block->user = NULL;

	block->virtMem = Mem_Alloc(bytes);

	block->vk_buffer = NULL;
	block->vk_offset = 0;

	return block;
}

vertCache_t	*idVertexCache::AllocFrameTemp_VK(void *data, int size, bool indexBuffer)
{
	vertCache_t	*block;

	if (size <= 0) {
		common->Error("idVertexCache::AllocFrameTemp_VK: size = %i\n", size);
	}

	vertCache_t *tempBlock;
	if (dynamicAllocThisFrame + size > frameBytes) {
		// if we don't have enough room in the temp block, allocate a static block,
		// but immediately free it so it will get freed at the next frame
		tempOverflow = true;

		tempBlock = NULL;
		vertCache_t *ptr = tempBuffers[listNum];
		while (ptr->next)
		{
			if ( frameBytes - ptr->next->offset >= size)
			{
				tempBlock = ptr->next;
				break;
			}
			ptr = ptr->next;
		}
		// alloc new temp block
		if (!tempBlock)
		{
			common->Printf("Alloc new temp buffer: %d\n", dynamicAllocThisFrame);
			tempBlock = CreateTempVbo_VK(frameBytes);
			ptr->next = tempBlock;
			tempBlock->prev = ptr;
		}
	}
	else
		tempBlock = tempBuffers[listNum];

	// this data is just going on the shared dynamic list

	// if we don't have any remaining unused headers, allocate some more
	if (freeDynamicHeaders.next == &freeDynamicHeaders) {

		for (int i = 0; i < EXPAND_HEADERS; i++) {
			block = headerAllocator.Alloc();
			block->next = freeDynamicHeaders.next;
			block->prev = &freeDynamicHeaders;
			block->next->prev = block;
			block->prev->next = block;
		}
	}

	// move it from the freeDynamicHeaders list to the dynamicHeaders list
	block = freeDynamicHeaders.next;
	block->next->prev = block->prev;
	block->prev->next = block->next;
	block->next = dynamicHeaders.next;
	block->prev = &dynamicHeaders;
	block->next->prev = block;
	block->prev->next = block;

#ifdef _MULTITHREAD
	block->virtMem = NULL;
	block->virtMemDirty = false;
#endif
	block->size = size;
	block->tag = TAG_TEMP;
	block->indexBuffer = indexBuffer;
	if (tempBlock == tempBuffers[listNum])
	{
		block->offset = dynamicAllocThisFrame;
	}
	else
	{
		block->offset = tempBlock->offset;
		tempBlock->offset += block->size;
	}
	dynamicAllocThisFrame += block->size;
	dynamicCountThisFrame++;
	block->user = NULL;
	block->frameUsed = 0;

	// copy the data
	block->virtMem = tempBlock->virtMem;
	block->vbo = tempBlock->vbo;
#ifdef _MULTITHREAD
	if(multithreadActive) // temp buffer will upload on ::BeginBackEnd() when start rendering this frame in render thread
	{
		SIMDProcessor->Memcpy((byte *)block->virtMem + block->offset, data, size);
		block->vk_buffer = NULL;
		block->vbo = VBO_INVALID_HANDLE;
		block->vk_offset = 0;
		block->virtMemDirty = true;
		return block;
	}
#endif
	VKBufferMap_t map;
	if (block->indexBuffer)
	{
		R_VK_AllocIndexBuffer(data, size, &map);
		BindIndexBuffer_VK(map.buffer->buffer);
	}
	else
	{
		R_VK_AllocVertexBuffer(data, size, &map);
		BindBuffer_VK(map.buffer->buffer);
		//printf("vvv %d %d %d %p\n", vk_backend.frameCount, map.offset, map.size, map.buffer);
	}
	block->vk_buffer = map.buffer;
	block->vbo = map.handle;
	block->vk_offset = map.offset;

	return block;
}

// always write to host mapped buffer
vertCache_t	*idVertexCache::AllocFrameTempInRendering_VK(void *data, int size, bool indexBuffer)
{
	static vertCache_t vertCache[2];

	vertCache_t	*block = &vertCache[indexBuffer ? 1 : 0];

	if (size <= 0) {
		common->Error("idVertexCache::AllocFrameTempInRendering_VK: size = %i\n", size);
	}

	block->next = NULL;
	block->prev = NULL;

#ifdef _MULTITHREAD
	block->virtMemDirty = false;
#endif
	block->size = size;
	block->tag = TAG_TEMP;
	block->indexBuffer = indexBuffer;
	block->offset = 0;
	block->user = NULL;
	block->frameUsed = 0;

	// copy the data
	block->virtMem = NULL;
	VKBufferMap_t map;
	if (block->indexBuffer)
	{
		R_VK_AllocIndexBuffer(data, size, &map);
		BindIndexBuffer_VK(map.buffer->buffer);
	}
	else
	{
		R_VK_AllocVertexBuffer(data, size, &map);
		BindBuffer_VK(map.buffer->buffer);
	}
	block->vk_buffer = map.buffer;
	block->vbo = map.handle;
	block->vk_offset = map.offset;

	return block;
}

void idVertexCache::TransferData_VK(vertCache_t *block)
{
	// copy the data
	if (block->vk_buffer && block->vk_buffer->buffer)
	{
		if (block->vk_buffer->size < block->size)
		{
			VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "free_buffer_%d::%d::%s_%s", vk_context.frameIndex, block->vbo, "static", block->indexBuffer ? "index" : "vertex");
			R_VK_DestroyBuffer(block->vbo);
			block->vk_buffer = NULL;
			block->vbo = VKHANDLE_INVALID;
		}
	}
	if (!block->vk_buffer)
	{
		block->vbo = VK_AllocBuffer(&block->vk_buffer);
	}
	if (VK_NULL_HANDLE == block->vk_buffer->buffer)
	{
		if (block->indexBuffer) {
			VK_CreateIndexBuffer(block->vk_buffer, block->size, false, false); // GPU only
		} else {
			VK_CreateVertexBuffer(block->vk_buffer, block->size, false, false); // GPU only
		}
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "buffer::%d::%s_%s", block->vbo, "static", block->indexBuffer ? "index" : "vertex");
	}
	R_VK_UploadBufferData(block->vk_buffer, block->size, block->virtMem, 0);
	if (block->indexBuffer) {
		BindIndexBuffer_VK(block->vk_buffer->buffer);
	} else {
		BindBuffer_VK(block->vk_buffer->buffer);
	}
	block->vk_offset = 0;
	block->virtMemDirty = false;
}

#ifdef _MULTITHREAD
void  idVertexCache::BeginBackEnd_VK(int which)
{
	// Transfer current frame temp data to GPU
	if(multithreadActive) // upload all used temp host memory: don't upload full memory once, because tempBuffers is linked by `->next` in vulkan, blocks of `TAG_TEMP` always not in `static Alloc()` through `tempOverflow == true`
	{
		for (vertCache_t *block = dynamicHeaderss[which].next ; block != &dynamicHeaderss[which] ; block = block->next) {
			VKBufferMap_t map;
			if (block->indexBuffer)
			{
				R_VK_AllocIndexBuffer((byte *)block->virtMem + block->offset, block->size, &map);
				BindIndexBuffer_VK(map.buffer->buffer);
			}
			else
			{
				R_VK_AllocVertexBuffer((byte *)block->virtMem + block->offset, block->size, &map);
				BindBuffer_VK(map.buffer->buffer);
			}
			block->vk_buffer = map.buffer;
			block->vbo = map.handle;
			block->vk_offset = map.offset;
		}
	}
}

void idVertexCache::EndBackEnd_VK(int which)
{
	if (multithreadActive)
	{
		for (vertCache_t *block = deferredFreeLists[which].next; block && block != &deferredFreeLists[which]; block = block->next) {
			// temp blocks are in a shared space that won't be freed
			if (block->tag != TAG_TEMP) {
				if (block->vk_buffer) {
					VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->vk_buffer->buffer, "free_buffer_%d::%d::%s_%s", vk_context.frameIndex, block->vbo, "static", block->indexBuffer ? "index" : "vertex");
					R_VK_DestroyBuffer(block->vbo);
					block->vbo = VBO_INVALID_HANDLE;
					block->vk_buffer = NULL;
				}
			}
		}
	}
}
#endif