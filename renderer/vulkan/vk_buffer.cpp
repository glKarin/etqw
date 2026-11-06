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
#define IDVK_STAGING_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkStagingBufferBlockSize.GetInteger())
#define IDVK_UNIFORM_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkUniformBufferBlockSize.GetInteger())
#define IDVK_VERTEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkVertexBufferBlockSize.GetInteger())
#define IDVK_INDEX_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(r_vkIndexBufferBlockSize.GetInteger())
#else
#define IDVK_STAGING_BUFFER_BLOCK_SIZE IDVK_BUFFER_BLOCK_SIZE_MB(64)
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
	int bufSize = MEM_ALIGN(size, block->alignment);
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
	int offset = MEM_ALIGN(block->offset, block->alignment);
	int diff = offset - block->offset;
	//size = MEM_ALIGN(size, block->alignment);
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
		cache->list = (VKBufferBlock_t *) malloc(sizeof(*cache->list) * cache->size);
		memset(cache->list, 0, sizeof(*cache->list) * cache->size);
	} else if (cache->num == cache->size) {
		VKBufferBlock_t *VKBufferBlock_t_oldData = cache->list;
		cache->size += cache->granularity;
		cache->list = (VKBufferBlock_t *) malloc(sizeof(*cache->list) * cache->size);
		memset(cache->list, 0, sizeof(*cache->list) * cache->size);
		memcpy(cache->list, VKBufferBlock_t_oldData, sizeof(*cache->list) * cache->num);
		free(VKBufferBlock_t_oldData);
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

	free(cache->list);
	VKPTRZERO(cache);
}

static void R_VK_FlushBufferQueue(VKBufferQueue_t *cache) {
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
		return;

	int numi = 0;
	block = &cache->list[0];
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaMappedMemoryRange *mappedMemoryRanges = (VmaMappedMemoryRange *)malloc(sizeof(*mappedMemoryRanges) * num);
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

	vmaFlushMappedMemoryRanges(vk_context.vmaAllocator, num, mappedMemoryRanges);
#else
	VkMappedMemoryRange *mappedMemoryRanges = (VkMappedMemoryRange *)malloc(sizeof(*mappedMemoryRanges) * num);
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

	free(mappedMemoryRanges);
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
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)commandPool->commandPool, "stageBuffer::%d::commandPool", handle);
	vk_buffer.stageBuffer.commandPool = commandPool;
	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandPool->commandBuffers[i], "stageBuffer::%d::commandBuffer%d", handle, i);
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_FENCE, (uint64_t)commandPool->fences[i], "stageBuffer::%d::fence%d", handle, i);
		for (int m = 0; m < ID_VK_NUM_STAGE_BUFFER; m++)
		{
			R_VK_InitBufferBlock(&vk_buffer.stageBuffer.buffers[i].buffers[m], BT_TRANSFER, IDVK_STAGING_BUFFER_BLOCK_SIZE);
		}
	}

	handle = VK_AllocCommandPool(&commandPool);
	VK_CreateCommandPool(commandPool, CU_TRANSFER, true, true);
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)commandPool->commandPool, "stageReadBuffer::%d::commandPool", handle);
	vk_buffer.stageReadBuffer.commandPool = commandPool;
	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandPool->commandBuffers[i], "stageReadBuffer::%d::commandBuffer%d", handle, i);
		VKDEBUG_SET_NAME(VK_OBJECT_TYPE_FENCE, (uint64_t)commandPool->fences[i], "stageReadBuffer::%d::fence%d", handle, i);
		for (int m = 0; m < ID_VK_NUM_STAGE_BUFFER; m++)
		{
			R_VK_InitBufferBlock(&vk_buffer.stageReadBuffer.buffers[i].buffers[m], BT_TRANSFER_DST, IDVK_STAGING_BUFFER_BLOCK_SIZE);
		}
	}

	vk_buffer.initialized = true;
}

void R_VK_ShutdownBuffer(void)
{
	if (!vk_buffer.initialized)
		return;

	Sys_Printf("[Vulkan]: Vertex buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxVertexAllocation, vk_buffer.counter.c_maxVertexBlock);
	Sys_Printf("[Vulkan]: Index buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxIndexAllocation, vk_buffer.counter.c_maxIndexBlock);
	Sys_Printf("[Vulkan]: Uniform buffer max used memory=%d, max used block=%d\n", vk_buffer.counter.c_maxUniformAllocation, vk_buffer.counter.c_maxUniformBlock);

	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		R_VK_FreeBufferQueue(&vk_buffer.vertexBuffers[i]);
		R_VK_FreeBufferQueue(&vk_buffer.indexBuffers[i]);
		R_VK_FreeBufferQueue(&vk_buffer.uniformBuffers[i]);
	}

	VKZERO(vk_buffer);

	vk_buffer.initialized = false;
}

static VkCommandBuffer R_VK_BeginStageCommandBuffer(VKStageBuffer_t *sbuffer)
{
	VkCommandBuffer commandBuffer = sbuffer->currentStageCommandBuffer;
	VKStageBufferQueue_t *queue = &sbuffer->currentStageBuffer;
	if (!queue->recording[queue->currentBuffer])
	{
		VkResult result;

		result = vkResetCommandBuffer(commandBuffer, 0);
		HANDLE_VK_ERROR_EXIT(result, "vkResetCommandBuffer");

		VkCommandBufferBeginInfo commandBufferBeginInfo;
		VKZERO(commandBufferBeginInfo);
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = 0; // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		HANDLE_VK_ERROR_EXIT(result, "vkBeginCommandBuffer");

		queue->recording[queue->currentBuffer] = true;
	}
	return commandBuffer;
}

static void R_VK_SubmitStageCommandBuffer(VKStageBuffer_t *sbuffer, int index)
{
	VkResult result;

	VkCommandBuffer commandBuffer = sbuffer->currentStageCommandBuffer;
	VKStageBufferQueue_t *queue = &sbuffer->currentStageBuffer;

	VKBufferBlock_t *block = &queue->buffers[index];
	if (queue->mapped[index])
	{
		R_VK_FlushBufferBlock(block);
		queue->mapped[index] = NULL;
	}

	bool needSubmit = false;
	if (queue->recording[index])
	{
		vkEndCommandBuffer(commandBuffer);
		queue->recording[index] = false;
		needSubmit = true;
	}

	if (needSubmit && !queue->submitted[index])
	{
		queue->submitted[index] = true;

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

		queue->submitted[index] = false;

		R_VK_ResetBufferBlock(&queue->buffers[index]);
	}
}

static bool R_VK_AllocStageBuffer(VKStageBuffer_t *sbuffer, int size, VKBufferMap_t *map)
{
	VKStageBufferQueue_t *queue = &sbuffer->currentStageBuffer;
	VKBufferBlock_t *block = NULL;

	int lastBuffer = queue->currentBuffer;
	bool found = false;

	if (queue->recording[queue->currentBuffer]) // 当前的正在记录命令
	{
		if (R_VK_AllocBufferBlock(&queue->buffers[queue->currentBuffer], size, map))
		{
			block = &queue->buffers[queue->currentBuffer];
			found = true;
		}

		// 未分配成功则尝试下一块
		if (!found)
		{
			queue->currentBuffer = (queue->currentBuffer + 1) % ID_VK_NUM_STAGE_BUFFER;
			if (queue->currentBuffer != lastBuffer)
			{
				R_VK_SubmitStageCommandBuffer(sbuffer, queue->currentBuffer); // 提交之前的

				R_VK_BeginStageCommandBuffer(sbuffer); // 开启新的
				if (R_VK_AllocBufferBlock(&queue->buffers[queue->currentBuffer], size, map))
				{
					block = &queue->buffers[queue->currentBuffer];
					found = true;
				}
			}
		}
	}
	else
	{
		R_VK_BeginStageCommandBuffer(sbuffer);
		if (R_VK_AllocBufferBlock(&queue->buffers[queue->currentBuffer], size, map))
		{
			block = &queue->buffers[queue->currentBuffer];
			found = true;
		}
	}

	if (!found || !block) // 分配缓冲区内存失败
	{
		common->Error("R_VK_AllocStageBuffer: Alloc fail: size=%d, recording=%d, frame=%d->%d\n", size, queue->recording[queue->currentBuffer], lastBuffer, queue->currentBuffer);
		return false;
	}

	if (!queue->mapped[queue->currentBuffer])
	{
		queue->mapped[queue->currentBuffer] = block->ptr;
	}

	return true;
}

bool R_VK_AllocStageWriteBuffer(int size, VKBufferMap_t *map)
{
	return R_VK_AllocStageBuffer(&vk_buffer.stageBuffer, size, map);
}

bool R_VK_AllocStageReadBuffer(int size, VKBufferMap_t *map)
{
	return R_VK_AllocStageBuffer(&vk_buffer.stageReadBuffer, size, map);
}

static void R_VK_BeginStageBuffer(VKStageBuffer_t *sbuffer)
{
    for (int i = 0; i < ID_VK_NUM_STAGE_BUFFER; i++)
    {
        sbuffer->currentStageBuffer.currentBuffer = 0;
    }
	//vkResetFences(vk_context.device, 1, &sbuffer->commandPool->fences[vk_context.frameIndex]);
}

static void R_VK_BeginStageBuffers()
{
	R_VK_BeginStageBuffer(&vk_buffer.stageBuffer);
	R_VK_BeginStageBuffer(&vk_buffer.stageReadBuffer);
}

static void R_VK_EndStageBuffer(VKStageBuffer_t *sbuffer)
{
	for (int i = 0; i < ID_VK_NUM_STAGE_BUFFER; i++)
	{
		R_VK_SubmitStageCommandBuffer(sbuffer, i);
	}
	sbuffer->currentStageBuffer.currentBuffer = 0;
}

static void R_VK_EndStageBuffers()
{
	R_VK_EndStageBuffer(&vk_buffer.stageBuffer);
	R_VK_EndStageBuffer(&vk_buffer.stageReadBuffer);
}

void R_VK_EndStageReadBuffer()
{
	R_VK_EndStageBuffer(&vk_buffer.stageReadBuffer);
}

void R_VK_BeginStageReadBuffer()
{
	R_VK_BeginStageBuffer(&vk_buffer.stageReadBuffer);
}

void R_VK_VertexCacheBeginFrame()
{
	R_VK_ResetBufferQueue(&vk_buffer.vertexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.indexBuffers[vk_context.frameIndex]);
	R_VK_ResetBufferQueue(&vk_buffer.uniformBuffers[vk_context.frameIndex]);

	R_VK_BeginStageBuffers();
}

void R_VK_VertexCacheEndFrame()
{
	R_VK_FlushBufferQueue(&vk_buffer.vertexBuffers[vk_context.frameIndex]);
	R_VK_FlushBufferQueue(&vk_buffer.indexBuffers[vk_context.frameIndex]);
	R_VK_FlushBufferQueue(&vk_buffer.uniformBuffers[vk_context.frameIndex]);

	vk_buffer.counter.c_maxVertexAllocation = idMath::Max(vk_buffer.counter.c_maxVertexAllocation, vk_buffer.vertexBuffers[vk_context.frameIndex].usedMemory);
	vk_buffer.counter.c_maxIndexAllocation = idMath::Max(vk_buffer.counter.c_maxIndexAllocation, vk_buffer.indexBuffers[vk_context.frameIndex].usedMemory);
	vk_buffer.counter.c_maxUniformAllocation = idMath::Max(vk_buffer.counter.c_maxUniformAllocation, vk_buffer.uniformBuffers[vk_context.frameIndex].usedMemory);

	vk_buffer.counter.c_maxVertexBlock = idMath::Max(vk_buffer.counter.c_maxVertexBlock, vk_buffer.vertexBuffers[vk_context.frameIndex].num);
	vk_buffer.counter.c_maxIndexBlock = idMath::Max(vk_buffer.counter.c_maxIndexBlock, vk_buffer.indexBuffers[vk_context.frameIndex].num);
	vk_buffer.counter.c_maxUniformBlock = idMath::Max(vk_buffer.counter.c_maxUniformBlock, vk_buffer.uniformBuffers[vk_context.frameIndex].num);

	R_VK_EndStageBuffers();
}

static void * R_VK_MapBufferQueue(VKBufferQueue_t *cache, const VKBufferMap_t *map) {
	assert(cache->map.buffer);
	void *ptr = VK_MapBuffer(map->buffer, map->offset, map->size);
	cache->map = *map;
	cache->map.ptr = ptr;
	return ptr;
}

static void R_VK_UnmapBufferQueue(VKBufferQueue_t *cache) {
	if(cache->map.ptr)
	{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
		vmaUnmapMemory(vk_context.vmaAllocator, cache->map.buffer->vmaAllocation);
#else
		vkUnmapMemory(vk_context.device, cache->map.buffer->memory);
#endif
		VKZERO(cache->map);
	}
}
