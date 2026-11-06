#ifndef _VULKAN_VKBUFFER_H
#define _VULKAN_VKBUFFER_H

#define currentStageWriteBuffer stageBuffer.buffers[vk_context.frameIndex]
#define currentStageWriteCommandBuffer stageBuffer.commandPool->commandBuffers[vk_context.frameIndex]

#define currentStageReadBuffer stageReadBuffer.buffers[vk_context.frameIndex]
#define currentStageReadCommandBuffer stageReadBuffer.commandPool->commandBuffers[vk_context.frameIndex]

#define currentStageBuffer buffers[vk_context.frameIndex]
#define currentStageCommandBuffer commandPool->commandBuffers[vk_context.frameIndex]

#define currentVertexBuffer vertexBuffers[vk_context.frameIndex]
#define currentIndexBuffer indexBuffers[vk_context.frameIndex]
#define currentUniformBuffer uniformBuffers[vk_context.frameIndex]

/**
 * 缓冲区类型
 */
typedef enum VKBufferType_e
{
    BT_VERTEX,
    BT_INDEX,
    BT_UNIFORM,
    BT_TRANSFER,
    BT_TRANSFER_DST,
    BT_VERTEX_DST,
    BT_INDEX_DST,
    BT_UNIFORM_DST,
} VKBufferType_s;

/**
 * 缓冲区分配映射结果
 */
typedef struct VKBufferMap_s
{
    VKHandle_t handle; // 使用的缓冲区上下文句柄
    VKBuffer_t *buffer; // 使用的缓冲区对象
    int offset; // 本次分配的缓冲区偏移
    int size; // 本次分配的缓冲区大小(对齐后)
    void *ptr; // 本次分配的缓冲区映射地址
} VKBufferMap_t;

/**
 * 单个缓冲区块
 */
typedef struct VKBufferBlock_s {
    VKBuffer_t *buffer; // 缓冲区对象
    void *ptr; // 映射地址
    int offset; // 当前偏移
    int remain; // 剩余大小
    int alignment; // 对齐大小
    int type; // 类型
} VKBufferBlock_t;

/**
 * 缓冲区队列(每帧偏移重置为0, 只能追加不能删除)
 */
typedef struct VKBufferQueue_s {
    VKBufferBlock_s *list; // 缓冲区块列表
    int size; // 缓冲区块数量
    int num; // 使用的缓冲区块
    int granularity; // 扩容时的增量
    int blockSize; // 块大小
    int bufferType; // 缓冲区类型
    VKBufferMap_t map; // 当前映射的缓冲区块

    int usedMemory; // 本次使用量
} VKBufferQueue_t;

#define ID_VK_NUM_STAGE_BUFFER 2

/**
 * Staging缓冲区队列(缓冲区块大小固定, 无法分配时强制提交到队列)
 */
typedef struct VKStageBufferQueue_s
{
    VKBufferBlock_t buffers[ID_VK_NUM_STAGE_BUFFER]; // 缓冲区组
    bool recording[ID_VK_NUM_STAGE_BUFFER]; // 当前是否命令记录中, 为true则需要结束命令和提交命令
    bool submitted[ID_VK_NUM_STAGE_BUFFER]; // 当前命令是否提交, 为false则未进行提交
    void *mapped[ID_VK_NUM_STAGE_BUFFER]; // 当前缓冲区映射的地址, 为NULL则不显式刷新
    int currentBuffer; // 当前帧使用的缓冲区
} VKStageBufferQueue_t;

/**
 * Staging缓冲区
 */
typedef struct VKStageBuffer_s
{
    VKCommandPool_t *commandPool;
    VKStageBufferQueue_t buffers[NUM_INFLIGHT_FRAME];
} VKStageBuffer_t;

typedef struct VKBufferObject_s
{
    VKBufferQueue_t uniformBuffers[NUM_INFLIGHT_FRAME];
    VKBufferQueue_t vertexBuffers[NUM_INFLIGHT_FRAME];
    VKBufferQueue_t indexBuffers[NUM_INFLIGHT_FRAME];

    VKStageBuffer_t stageBuffer;
    VKStageBuffer_t stageReadBuffer;

    struct {
        int c_maxVertexAllocation;
        int c_maxIndexAllocation;
        int c_maxUniformAllocation;
        int c_maxVertexBlock;
        int c_maxIndexBlock;
        int c_maxUniformBlock;
    } counter;

    bool initialized;
} VKBufferObject_t;

void R_VK_InitBuffer(void);
void R_VK_ShutdownBuffer(void);
void R_VK_AllocBufferQueue(VKBufferQueue_t *cache, int size, VKBufferMap_t *map);
void R_VK_VertexCacheBeginFrame();
void R_VK_VertexCacheEndFrame();
bool R_VK_AllocStageWriteBuffer(int size, VKBufferMap_t *map);
bool R_VK_AllocStageReadBuffer(int size, VKBufferMap_t *map);
void R_VK_BeginStageReadBuffer();
void R_VK_EndStageReadBuffer();

extern VKBufferObject_t vk_buffer;

#endif //_VULKAN_VKBUFFER_H