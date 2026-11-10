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
 * 缓冲区队列(每帧刷新后偏移重置为0, 只能追加不能删除)
 */
typedef struct VKBufferQueue_s {
    VKBufferBlock_s *list; // 缓冲区块列表
    int size; // 缓冲区块数量
    int num; // 使用的缓冲区块
    int granularity; // 扩容时的增量
    int blockSize; // 块大小
    int bufferType; // 缓冲区类型

    int usedMemory; // 本次使用量
} VKBufferQueue_t;

/**
 * Staging缓冲区队列
 */
typedef struct VKStageBufferQueue_s
{
    VKBufferQueue_t blocks; // 缓冲区组
    bool recording; // 当前是否命令记录中, 为true则需要结束命令和提交命令
} VKStageBufferQueue_t;

/**
 * Staging缓冲区
 */
typedef struct VKStageBuffer_s
{
    VKCommandPool_t *commandPool; // 命令池
    VKStageBufferQueue_t buffers[NUM_INFLIGHT_FRAME];
} VKStageBuffer_t;

/**
 * Vulkan全局缓冲区
 */
typedef struct VKBufferObject_s
{
    VKBufferQueue_t uniformBuffers[NUM_INFLIGHT_FRAME]; // Uniform数据上传缓冲区(映射到Host/手动刷新)
    VKBufferQueue_t vertexBuffers[NUM_INFLIGHT_FRAME]; // 顶点数据上传缓冲区(映射到Host/手动刷新)
    VKBufferQueue_t indexBuffers[NUM_INFLIGHT_FRAME]; // 索引数据上传缓冲区(映射到Host/手动刷新)

    VKStageBuffer_t stageBuffer; // 上传staging缓冲区(映射到Host/手动刷新)
    VKStageBuffer_t stageReadBuffer; // 读取staging缓冲区(映射到Host/手动刷新)

    // 计数器
    struct {
        int c_maxVertexAllocation;
        int c_maxIndexAllocation;
        int c_maxUniformAllocation;
        int c_maxWriteStagingAllocation;
        int c_maxReadStagingAllocation;
        int c_maxVertexBlock;
        int c_maxIndexBlock;
        int c_maxUniformBlock;
        int c_maxWriteStagingBlock;
        int c_maxReadStagingBlock;
    } counter;

    bool initialized; // 是否初始化
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
