#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"


#define IDVK_UNIFORM_BUFFER_CACHE_INIT_SIZE 1
#define IDVK_UNIFORM_BUFFER_CACHE_GRANULARITY 1

#define IDVK_VERTEX_BUFFER_CACHE_INIT_SIZE 1
#define IDVK_VERTEX_BUFFER_CACHE_GRANULARITY 1

#define IDVK_INDEX_BUFFER_CACHE_INIT_SIZE 1
#define IDVK_INDEX_BUFFER_CACHE_GRANULARITY 1

#define IDVK_BUFFER_BLOCK_SIZE_MB(x) ( (x) * 1024 * 1024 )

#if IDVK_CONTAINER_SIZE_CONFIG
#define IDVK_STAGING_WRITE_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkStagingWriteBufferBlockSize.GetInteger())
#define IDVK_STAGING_READ_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkStagingReadBufferBlockSize.GetInteger())
#define IDVK_UNIFORM_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkUniformBufferBlockSize.GetInteger())
#define IDVK_VERTEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkVertexBufferBlockSize.GetInteger())
#define IDVK_INDEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkIndexBufferBlockSize.GetInteger())
#else
#define IDVK_STAGING_WRITE_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(64)
#define IDVK_STAGING_READ_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(16)
#define IDVK_UNIFORM_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(64)
#define IDVK_VERTEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(64)
#define IDVK_INDEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(16)
#endif

VKBufferObject_t vk_buffer;

static void R_VK_InitBufferBlock(VKBufferBlock_t *block, int type, int size) {
	VKPTRZERO(block);
	switch(type)
	{
		case BT_VERTEX:
		case BT_VERTEX_DST:
			block->alignment = vk_context.physicalDeviceProperties.limits.minStorageBufferOffsetAlignment;
			break;
		case BT_INDEX:
		case BT_INDEX_DST:
			block->alignment = vk_context.physicalDeviceProperties.limits.minStorageBufferOffsetAlignment;
			break;
		case BT_UNIFORM:
		case BT_UNIFORM_DST:
			block->alignment = vk_context.physicalDeviceProperties.limits.minUniformBufferOffsetAlignment;
			break;
		case BT_TRANSFER:
		case BT_TRANSFER_DST:
			block->alignment = vk_context.physicalDeviceProperties.limits.optimalBufferCopyOffsetAlignment;
			break;
		default:
			Sys_Error("Unexpected buffer type: %d\n", type);
			return;
	}
	int bufSize = VKMEM_ALIGN(size, block->alignment);
	Sys_Printf("Create buffer block size: %d -> %d(alignment: %d) %d - %d\n", size, bufSize, block->alignment, vk_context.frameIndex, type);
	VKHandle_t handle = VK_AllocBuffer(&block->buffer);
	switch(type)
	{
		case BT_VERTEX:
			VK_CreateVertexBuffer(block->buffer, bufSize, true, false);
			break;
		case BT_VERTEX_DST:
			VK_CreateVertexBuffer(block->buffer, bufSize, false, false);
			break;
		case BT_INDEX:
			VK_CreateIndexBuffer(block->buffer, bufSize, true, false);
			break;
		case BT_INDEX_DST:
			VK_CreateIndexBuffer(block->buffer, bufSize, false, false);
			break;
		case BT_UNIFORM:
			VK_CreateUniformBuffer(block->buffer, bufSize, true, false);
			break;
		case BT_UNIFORM_DST:
			VK_CreateUniformBuffer(block->buffer, bufSize, false, false);
			break;
		case BT_TRANSFER:
			VK_CreateTransferSrcBuffer(block->buffer, bufSize, true, false);
			break;
		case BT_TRANSFER_DST:
			VK_CreateTransferDstBuffer(block->buffer, bufSize, true, false);
			break;
		default:
			Sys_Error("Unexpected buffer type: %d\n", type);
			return;
	}
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)block->buffer->buffer, "tempBuffer::buffer%d_%d", handle, type);
	block->remain = block->buffer->size;
	block->type = type;

	//const bool mapping = type <= BT_TRANSFER_DST;
	if (block->buffer->map)
		block->ptr = VK_MapBuffer(block->buffer, 0, block->buffer->size);
	else
		block->ptr = NULL;
}

static bool R_VK_AllocBufferBlock(VKBufferBlock_t *block, int size, VKBufferMap_t *map) {
	if(size > block->buffer->size)
	{
		common->FatalError("[Vulkan]: Allocate %d bytes memory must be less than %d bytes of block in one allocation\n", size, block->buffer->size);
		return false;
	}
	int offset = VKMEM_ALIGN(block->offset, block->alignment);
	int diff = offset - block->offset;
	//size = VKMEM_ALIGN(size, block->alignment);
	if (block->remain - diff < size)
		return false;
	block->offset = offset;
	block->offset += size;
	block->remain -= size + diff;
	map->offset = offset;
	map->size = size;
	map->buffer = block->buffer;
	map->ptr = (char *)block->ptr + offset;
	return true;
}

static void R_VK_FreeBufferBlock(VKBufferBlock_t *block) {
	if(block->ptr)
	{
		VK_UnmapBuffer(block->buffer);
	}
	VKPTRZERO(block);
}

static void R_VK_ResetBufferBlock(VKBufferBlock_t *block) {
	block->offset = 0;
	block->remain = block->buffer->size;
}

static void R_VK_FlushBufferBlock(VKBufferBlock_t *block) {
	if(block->ptr && block->offset > 0)
	{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
		VmaMappedMemoryRange mappedMemoryRange;
		VKZERO(mappedMemoryRange);
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedMemoryRange.offset = 0;
		mappedMemoryRange.size = block->offset;
		mappedMemoryRange.allocation = block->buffer->vmaAllocation;
		if (block->type == BT_TRANSFER_DST)
			vmaInvalidateMappedMemoryRanges(vk_context.vmaAllocator, 1, &mappedMemoryRange);
		else
			vmaFlushMappedMemoryRanges(vk_context.vmaAllocator, 1, &mappedMemoryRange);
#else
		VkMappedMemoryRange mappedMemoryRange;
		VKZERO(mappedMemoryRange);
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedMemoryRange.offset = 0;
		mappedMemoryRange.size = block->offset;
		mappedMemoryRange.memory = block->buffer->memory;
		vkFlushMappedMemoryRanges(vk_context.device, 1, &mappedMemoryRange);
#endif
	}
}

static void R_VK_InitBufferQueue(VKBufferQueue_t *cache, int type, int size, int granularity, int blockSize) {
	VKPTRZERO(cache);
	cache->size = size;
	cache->granularity = granularity;
	cache->blockSize = blockSize;
	cache->bufferType = type;
}

void R_VK_AllocBufferQueue(VKBufferQueue_t *cache, int size, VKBufferMap_t *map)
{
	VKBufferBlock_t *item = &cache->list[0];
	for (int i = 0; i < cache->num; i++, item++) {
		if(R_VK_AllocBufferBlock(item, size, map))
		{
			return;
		}
	}

	if (!cache->list) {
		cache->list = (VKBufferBlock_t *) VKMALLOCT(sizeof(*cache->list) * cache->size, VKBufferQueue_t::ALLOC);
		memset(cache->list, 0, sizeof(*cache->list) * cache->size);
	} else if (cache->num == cache->size) {
		VKBufferBlock_t *VKBufferBlock_t_oldData = cache->list;
		cache->size += cache->granularity;
		cache->list = (VKBufferBlock_t *) VKMALLOCT(sizeof(*cache->list) * cache->size, VKBufferQueue_t::RESIZE);
		memset(cache->list, 0, sizeof(*cache->list) * cache->size);
		memcpy(cache->list, VKBufferBlock_t_oldData, sizeof(*cache->list) * cache->num);
		VKFREE(VKBufferBlock_t_oldData);
	}
	int index = cache->num;
	item = &cache->list[index];
	cache->num++;

	R_VK_InitBufferBlock(item, cache->bufferType, cache->blockSize);

	R_VK_AllocBufferBlock(item, size, map);
}

static void R_VK_ResetBufferQueue(VKBufferQueue_t *cache) {
	VKBufferBlock_t *item = &cache->list[0];
	for (int i = 0; i < cache->num; i++, item++) {
		R_VK_ResetBufferBlock(item);
	}
}

static void R_VK_FreeBufferQueue(VKBufferQueue_t *cache) {
	VKBufferBlock_t *item = &cache->list[0];
	for (int i = 0; i < cache->num; i++, item++) {
		R_VK_FreeBufferBlock(item);
	}

	VKFREE(cache->list);
	VKPTRZERO(cache);
}

static bool R_VK_FlushBufferQueue(VKBufferQueue_t *cache) {
	int num = 0;
	cache->usedMemory = 0;
	VKBufferBlock_t *block = &cache->list[0];
	for (int i = 0; i < cache->num; i++, block++) {
		if(block->ptr && block->offset > 0)
		{
			cache->usedMemory += block->offset;
			num++;
		}
	}

	if(num == 0)
		return false;

	int numi = 0;
	block = &cache->list[0];
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaMappedMemoryRange *mappedMemoryRanges = (VmaMappedMemoryRange *)VKMALLOCT(sizeof(*mappedMemoryRanges) * num, VKBufferQueue_t::VmaMappedMemoryRange);
	VKPTRNZERO(mappedMemoryRanges, num);
	for (int i = 0; i < cache->num; i++, block++) {
		if(block->ptr && block->offset > 0)
		{
			mappedMemoryRanges[numi].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			mappedMemoryRanges[numi].offset = 0;
			mappedMemoryRanges[numi].size = block->offset;
			mappedMemoryRanges[numi].allocation = block->buffer->vmaAllocation;

			numi++;
		}
	}

	if (cache->bufferType == BT_TRANSFER_DST)
		vmaInvalidateMappedMemoryRanges(vk_context.vmaAllocator, num, mappedMemoryRanges);
	else
		vmaFlushMappedMemoryRanges(vk_context.vmaAllocator, num, mappedMemoryRanges);
#else
	VkMappedMemoryRange *mappedMemoryRanges = (VkMappedMemoryRange *)VKMALLOCT(sizeof(*mappedMemoryRanges) * num, VKBufferQueue_t::VmaMappedMemoryRange);
	VKPTRNZERO(mappedMemoryRanges, num);
	for (int i = 0; i < cache->num; i++, block++) {
		if(block->ptr && block->offset > 0)
		{
			mappedMemoryRanges[numi].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			mappedMemoryRanges[numi].offset = 0;
			mappedMemoryRanges[numi].size = block->offset;
			mappedMemoryRanges[numi].memory = block->buffer->memory;

			numi++;
		}
	}

	vkFlushMappedMemoryRanges(vk_context.device, num, mappedMemoryRanges);
#endif

	VKFREE(mappedMemoryRanges);

	return true;
}

void R_VK_InitBuffer(void)
{
	if (vk_buffer.initialized)
		return;

	VKZERO(vk_buffer);

	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		R_VK_InitBufferQueue(&vk_buffer.vertexBuffers[i], BT_VERTEX, IDVK_VERTEX_BUFFER_CACHE_INIT_SIZE, IDVK_VERTEX_BUFFER_CACHE_GRANULARITY, IDVK_VERTEX_BUFFER_BLOCK_SIZE);
		R_VK_InitBufferQueue(&vk_buffer.indexBuffers[i], BT_INDEX, IDVK_INDEX_BUFFER_CACHE_INIT_SIZE, IDVK_INDEX_BUFFER_CACHE_GRANULARITY, IDVK_INDEX_BUFFER_BLOCK_SIZE);
		R_VK_InitBufferQueue(&vk_buffer.uniformBuffers[i], BT_UNIFORM, IDVK_UNIFORM_BUFFER_CACHE_INIT_SIZE, IDVK_UNIFORM_BUFFER_CACHE_GRANULARITY, IDVK_UNIFORM_BUFFER_BLOCK_SIZE);
	}

	VKCommandPool_t *commandPool;
	VKHandle_t handle = VK_AllocCommandPool(&commandPool);
	VK_CreateCommandPool(commandPool, CU_TRANSFER, true, true);
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)commandPool->commandPool, "stagingUploadBuffer::%d::commandPool", handle);
	vk_buffer.stagingUploadBuffer.commandPool = commandPool;
	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandPool->commandBuffers[i], "stagingUploadBuffer::%d::commandBuffer%d", handle, i);
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_FENCE, (uint64_t)commandPool->fences[i], "stagingUploadBuffer::%d::fence%d", handle, i);
		R_VK_InitBufferQueue(&vk_buffer.stagingUploadBuffer.buffers[i].blocks, BT_TRANSFER, 1, 1, IDVK_STAGING_WRITE_BUFFER_BLOCK_SIZE);
	}

	handle = VK_AllocCommandPool(&commandPool);
	VK_CreateCommandPool(commandPool, CU_TRANSFER, true, true);
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)commandPool->commandPool, "stagingReadBuffer::%d::commandPool", handle);
	vk_buffer.stagingReadBuffer.commandPool = commandPool;
	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandPool->commandBuffers[i], "stagingReadBuffer::%d::commandBuffer%d", handle, i);
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_FENCE, (uint64_t)commandPool->fences[i], "stagingReadBuffer::%d::fence%d", handle, i);
		R_VK_InitBufferQueue(&vk_buffer.stagingReadBuffer.buffers[i].blocks, BT_TRANSFER_DST, 1, 1, IDVK_STAGING_READ_BUFFER_BLOCK_SIZE);
	}

	vk_buffer.initialized = true;
}

static void R_VK_PrintBuffer(void)
{
	common->Printf("[Vulkan]: Vertex buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxVertexAllocation, vk_buffer.counter.c_maxVertexBlock);
	common->Printf("[Vulkan]: Index buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxIndexAllocation, vk_buffer.counter.c_maxIndexBlock);
	common->Printf("[Vulkan]: Uniform buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxUniformAllocation, vk_buffer.counter.c_maxUniformBlock);
	common->Printf("[Vulkan]: Write staging buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxWriteStagingAllocation, vk_buffer.counter.c_maxWriteStagingBlock);
	common->Printf("[Vulkan]: Read staging buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxReadStagingAllocation, vk_buffer.counter.c_maxReadStagingBlock);
}

static void R_VK_FreeStagingBuffer(VKStagingBuffer_t *sbuffer)
{
	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		R_VK_FreeBufferQueue(&sbuffer->buffers[i].blocks);
	}
	VKPTRZERO(sbuffer);
}

void R_VK_ShutdownBuffer(void)
{
	if (!vk_buffer.initialized)
		return;

	R_VK_PrintBuffer();

	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		R_VK_FreeBufferQueue(&vk_buffer.vertexBuffers[i]);
		R_VK_FreeBufferQueue(&vk_buffer.indexBuffers[i]);
		R_VK_FreeBufferQueue(&vk_buffer.uniformBuffers[i]);
	}

	R_VK_FreeStagingBuffer(&vk_buffer.stagingUploadBuffer);
	R_VK_FreeStagingBuffer(&vk_buffer.stagingReadBuffer);

	VKZERO(vk_buffer);

	vk_buffer.initialized = false;
}

static void R_VK_BeginStageCommandBuffer(VKStagingBuffer_t *sbuffer)
{
	VKStagingBufferQueue_t *queue = &sbuffer->currentStageBuffer;
	if (!queue->recording)
	{
		VkResult result;
		VkCommandBuffer commandBuffer = sbuffer->currentStageCommandBuffer;

		result = vkResetCommandBuffer(commandBuffer, 0);
		HANDLE_VK_ERROR_EXIT(result, "vkResetCommandBuffer");

		VkCommandBufferBeginInfo commandBufferBeginInfo;
		VKZERO(commandBufferBeginInfo);
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = 0; // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		HANDLE_VK_ERROR_EXIT(result, "vkBeginCommandBuffer");

		queue->recording = true;
	}
}

static void R_VK_SubmitStageCommandBuffer(VKStagingBuffer_t *sbuffer)
{
	VkResult result;

	VkCommandBuffer commandBuffer = sbuffer->currentStageCommandBuffer;
	VKStagingBufferQueue_t *queue = &sbuffer->currentStageBuffer;

	bool needSubmit = false;
	if (queue->recording)
	{
		needSubmit = R_VK_FlushBufferQueue(&queue->blocks);

		vkEndCommandBuffer(commandBuffer);
		queue->recording = false;
	}

	if (needSubmit)
	{
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_VERTEX_INPUT_BIT};
		VkSubmitInfo submitInfo;
		VKZERO(submitInfo);
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		//vkResetFences(vk_context.device, 1, &sbuffer->commandPool->fences[vk_context.frameIndex]);
		result = vkQueueSubmit(vk_context.graphicsQueue.queue, 1, &submitInfo, sbuffer->commandPool->fences[vk_context.frameIndex]);
		HANDLE_VK_ERROR_EXIT(result, "vkQueueSubmit");

		VK_WaitFence(sbuffer->commandPool->fences[vk_context.frameIndex]);

		R_VK_ResetBufferQueue(&queue->blocks);
	}
}

static bool R_VK_AllocStageBuffer(VKStagingBuffer_t *sbuffer, int size, VKBufferMap_t *map)
{
	VKStagingBufferQueue_t *queue = &sbuffer->currentStageBuffer;

	R_VK_BeginStageCommandBuffer(sbuffer);

	R_VK_AllocBufferQueue(&queue->blocks, size, map);

	return true;
}

bool R_VK_AllocStageWriteBuffer(int size, VKBufferMap_t *map)
{
	return R_VK_AllocStageBuffer(&vk_buffer.stagingUploadBuffer, size, map);
}

bool R_VK_AllocStageReadBuffer(int size, VKBufferMap_t *map)
{
	return R_VK_AllocStageBuffer(&vk_buffer.stagingReadBuffer, size, map);
}

static void R_VK_BeginStageBuffer(VKStagingBuffer_t *sbuffer)
{
#if 0
	VKStagingBufferQueue_t *queue = &sbuffer->currentStageBuffer;

	R_VK_ResetBufferQueue(&queue->blocks);
	queue->recording = false;
#endif
}

static void R_VK_BeginStageBuffers()
{
	R_VK_BeginStageBuffer(&vk_buffer.stagingUploadBuffer);
	R_VK_BeginStageBuffer(&vk_buffer.stagingReadBuffer);
}

static void R_VK_EndStageBuffer(VKStagingBuffer_t *sbuffer)
{
	R_VK_SubmitStageCommandBuffer(sbuffer);
}

static void R_VK_EndStageBuffers()
{
	R_VK_EndStageBuffer(&vk_buffer.stagingUploadBuffer);
	R_VK_EndStageBuffer(&vk_buffer.stagingReadBuffer);
}

void R_VK_EndStageReadBuffer()
{
	R_VK_EndStageBuffer(&vk_buffer.stagingReadBuffer);
}

void R_VK_BeginStageReadBuffer()
{
	R_VK_BeginStageBuffer(&vk_buffer.stagingReadBuffer);
}

void R_VK_VertexCacheBeginFrame()
{
#if 0
	R_VK_ResetBufferQueue(&vk_buffer.vertexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.indexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.uniformBuffers[vk_context.frameIndex]);

	R_VK_BeginStageBuffers();
#endif
}

void R_VK_VertexCacheEndFrame()
{
	R_VK_FlushBufferQueue(&vk_buffer.vertexBuffers[vk_context.frameIndex]);
	R_VK_FlushBufferQueue(&vk_buffer.indexBuffers[vk_context.frameIndex]);
	R_VK_FlushBufferQueue(&vk_buffer.uniformBuffers[vk_context.frameIndex]);

	vk_buffer.counter.c_maxVertexAllocation = idMath::Max(vk_buffer.counter.c_maxVertexAllocation, vk_buffer.vertexBuffers[vk_context.frameIndex].usedMemory);
	vk_buffer.counter.c_maxIndexAllocation = idMath::Max(vk_buffer.counter.c_maxIndexAllocation, vk_buffer.indexBuffers[vk_context.frameIndex].usedMemory);
	vk_buffer.counter.c_maxUniformAllocation = idMath::Max(vk_buffer.counter.c_maxUniformAllocation, vk_buffer.uniformBuffers[vk_context.frameIndex].usedMemory);
	vk_buffer.counter.c_maxWriteStagingAllocation = idMath::Max(vk_buffer.counter.c_maxWriteStagingAllocation, vk_buffer.currentStageWriteBuffer.blocks.usedMemory);
	vk_buffer.counter.c_maxReadStagingAllocation = idMath::Max(vk_buffer.counter.c_maxReadStagingAllocation, vk_buffer.currentStageReadBuffer.blocks.usedMemory);

	R_VK_ResetBufferQueue(&vk_buffer.vertexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.indexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.uniformBuffers[vk_context.frameIndex]);

	R_VK_EndStageBuffers();

	vk_buffer.counter.c_maxVertexBlock = idMath::Max(vk_buffer.counter.c_maxVertexBlock, vk_buffer.vertexBuffers[vk_context.frameIndex].num);
	vk_buffer.counter.c_maxIndexBlock = idMath::Max(vk_buffer.counter.c_maxIndexBlock, vk_buffer.indexBuffers[vk_context.frameIndex].num);
	vk_buffer.counter.c_maxUniformBlock = idMath::Max(vk_buffer.counter.c_maxUniformBlock, vk_buffer.uniformBuffers[vk_context.frameIndex].num);
	vk_buffer.counter.c_maxWriteStagingBlock = idMath::Max(vk_buffer.counter.c_maxWriteStagingBlock, vk_buffer.currentStageWriteBuffer.blocks.num);
	vk_buffer.counter.c_maxReadStagingBlock = idMath::Max(vk_buffer.counter.c_maxReadStagingBlock, vk_buffer.currentStageReadBuffer.blocks.num);
}

static void * R_VK_MapBufferQueue(VKBufferQueue_t *cache, VKBufferMap_t *map) {
	assert(map->buffer);
	void *ptr = VK_MapBuffer(map->buffer, map->offset, map->size);
	map->ptr = ptr;
	return ptr;
}

static void R_VK_UnmapBufferQueue(VKBufferQueue_t *cache, const VKBufferMap_t *map) {
	if(map->ptr)
	{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
		vmaUnmapMemory(vk_context.vmaAllocator, map->buffer->vmaAllocation);
#else
		vkUnmapMemory(vk_context.device, map->buffer->memory);
#endif
	}
}

void R_VK_ListBuffer(const idCmdArgs &args)
{
	R_VK_PrintBuffer();
}
