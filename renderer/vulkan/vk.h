#ifndef _VULKAN_VKCONTEXT_H
#define _VULKAN_VKCONTEXT_H

#include <stdint.h>

#define VK_NO_PROTOTYPES
//#define VK_ENABLE_BETA_EXTENSIONS
#include "Volk/volk.h"

#include "vma/vma.h"

#include "vk_container.h"

#define NUM_INFLIGHT_FRAME 2 // 3

#define IDVK_MAX_DESCRIPTOR_POOL_SETS 4096

typedef int VKHandle_t;
#define VKHANDLE_INVALID (-1)

#define HANDLE_MAKE_TEXT(result, ...) \
	char text[1024]; \
	text[0] = '\0'; \
	idStr::snPrintf(text, sizeof(text), __VA_ARGS__); \
	idStr::snPrintf(text + strlen(text), sizeof(text) - strlen(text), " -> result: %d\n", result);

#define HANDLE_VK_ERROR_EXIT(result, ...) \
    do { \
        if(result != VK_SUCCESS) { \
			HANDLE_MAKE_TEXT(result, __VA_ARGS__); \
            Sys_Error(text); \
        } \
    } while(0)

#define HANDLE_VK_ERROR_WARN(result, ...) \
    do { \
        if(result != VK_SUCCESS) { \
			HANDLE_MAKE_TEXT(result, __VA_ARGS__); \
            Sys_Printf(text); \
        } \
    } while(0)

#define HANDLE_VK_ERROR_RETURNV(result, ret, ...) do { \
        if(result != VK_SUCCESS) { \
			HANDLE_MAKE_TEXT(result, __VA_ARGS__); \
            Sys_Printf(text); \
            return ret; \
        } \
    } while(0)

#define HANDLE_VK_ERROR_RETURN(result, ...) do { \
        if(result != VK_SUCCESS) { \
			HANDLE_MAKE_TEXT(result, __VA_ARGS__); \
            Sys_Printf(text); \
			return; \
        } \
    } while(0)

#define COUNT_OF(arr) (sizeof(arr) / sizeof(arr[0]))

#define currentFrameCommandBuffer commandPool.commandBuffers[vk_context.frameIndex]

/**
 * 渲染通道阶段
 */
typedef enum VKRenderPassStage_e
{
	RS_DONT_CARE = 0, // 忽略之前的缓冲区数据
	RS_LOAD = 1 << 1, // 读取之前的缓冲区数据
} VKRenderPassStage_t;

/**
 * 图像用途
 */
typedef enum VKImageUsage_e
{
	IU_SAMPLER = 1, // 只读采样
	IU_ATTACHMENT = 1 << 1, // 帧缓冲区附件
} VKImageUsage_t;

/**
 * 命令池用途
 */
typedef enum VKCommandPoolUsage_e
{
    CU_PRESENT = 1, // 呈现
    CU_TRANSFER = 2, // 传输
} VKCommandPoolUsage_t;

/**
 * Vulkan队列
 */
typedef struct VKQueue_s
{
	uint32_t familyIndex; // 队列索引
	VkQueue queue; // 队列
} VKQueue_t;

/**
 * Vulkan缓冲区
 */
typedef struct VKBuffer_s
{
    VkBuffer buffer; // 缓冲区句柄
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaAllocation vmaAllocation;
#else
    VkDeviceMemory memory; // 缓冲区内存, 用来映射
#endif
    int size; // 缓冲区大小, 用来映射
	int alignment; // 缓冲区对齐大小
	bool map; // 是否可以映射
	bool flush; // 是否自动刷新
} VKBuffer_t;

/**
 * Vulkan图像
 */
typedef struct VKImage_s
{
    VkImage image; // 图像句柄
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaAllocation vmaAllocation;
#else
    VkDeviceMemory memory; // 图像内存
#endif
    VkImageView imageView; // 图像视图
    VkImageView imageViewSecondary; // 图像视图2(用于D24S8格式的模版采样)
    VkSampler sampler; // 采样器(引用的共享)
    VkFormat format; // 图像格式
	VkImageLayout layout; // 
    int width, height; // 图像大小
	int mipLevels; // 多重细节层数量
    int layers; // 层数
    int usage; // 用法
    bool cubeMap; // 类型
} VKImage_t;

/**
 * Vulkan帧缓冲区
 */
typedef struct VKFramebuffer_s
{
    VkFramebuffer framebuffer; // 帧缓冲区句柄
    VKImage_t image; // 颜色图像
    VKImage_t depthStencilImage; // 深度+模版图像
} VKFramebuffer_t;

/**
 * Vulkan交换链
 */
typedef struct VKSwapchain_s
{
    VkSwapchainKHR swapchain; // 交换链句柄
    VKFramebuffer_t *framebuffers; // 帧缓冲区
    int numImages; // 交换链图像数量
    VkFormat format; // 颜色像素格式
    VkColorSpaceKHR colorSpace; // 颜色空间
    int width, height; // 大小
    uint32_t imageIndex; // 当前呈现的图像索引
    VKFramebuffer_t *loadFramebuffers; // 帧缓冲区(OP_LOAD)
} VKSwapchain_t;

/**
 * Vulkan命令池
 */
typedef struct VKCommandPool_s
{
    VkCommandPool commandPool; // 命令池句柄
    VkCommandBuffer commandBuffers[NUM_INFLIGHT_FRAME]; // 命令缓冲区
	VkFence fences[NUM_INFLIGHT_FRAME];
} VKCommandPool_t;

/**
 * Vulkan描述符池
 */
typedef struct VKDescriptorPool_s
{
    VkDescriptorPool descriptorPool; // 描述符池句柄
    VkDescriptorSet descriptorSets[IDVK_MAX_DESCRIPTOR_POOL_SETS]; // 分配的描述符集
    int usedDescriptorSets; // 已分配的描述符集数量
    int usedUniforms; // 已分配的描述符集中UBO数量
    int usedSamplers; // 已分配的描述符集中采样器数量
} VKDescriptorPool_t;

/**
 * Vulkan渲染管线布局模版
 */
typedef struct VKPipelineLayout_s
{
	VkShaderModule vertexShader; // 顶点着色器模块
	VkShaderModule fragmentShader; // 片段着色器模块
	VkPipelineLayout pipelineLayout; // 管线布局
	VkDescriptorSetLayout descriptorSetLayout; // 描述符集布局
} VKPipelineLayout_t;

/**
 * Vulkan渲染管线
 */
typedef struct VKPipeline_s
{
    VkPipeline pipeline; // 管线句柄
    VKPipelineLayout_t pipelineLayout; // 管线布局(副本引用)
} VKPipeline_t;

/**
 * Vulkan查询池
 */
typedef struct VKQueryPool_s
{
	VkQueryPool queryPool; // 查询池句柄
	int numQuery; // 总数量
	int usedQuery; // 使用的数量
} VKQueryPool_t;

/**
 * Vulkan全局环境
 */
typedef struct VKContext_s
{
    VkInstance instance; // Vulkan实例
    VkSurfaceKHR surface; // 图层
    VkPhysicalDevice physicalDevice; // 物理设备
    VkDevice device; // 逻辑设备
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaAllocator vmaAllocator; // VMA内存分配器
#endif

	VkSurfaceCapabilitiesKHR surfaceCapabilities; // 图层能力
	VkPhysicalDeviceProperties physicalDeviceProperties; // 物理设备属性
	VkPhysicalDeviceFeatures physicalDeviceFeatures; // 物理设备特性

    char **extensions; // 实例支持的扩展
    int numExtensions;
    char **validationLayers; // 实例支持的验证层
	int numValidationLayers;
	VkPhysicalDevice *physicalDevices; // 物理设备
	int numPhysicalDevices;
	char **physicalExtensions; // 物理设备支持的扩展
	int numPhysicalExtensions;
	char **physicalValidationLayers; // 物理设备支持的验证层
	int numPhysicalValidationLayers;

	VkAllocationCallbacks *allocator; // 内存分配器(未使用, 为NULL)

    VKQueue_t graphicsQueue; // 图形队列
    VKQueue_t transferQueue; // 传输队列
    VKQueue_t presentQueue; // 回显队列

    VkRenderPass renderPass; // 主渲染通道, 目标是交换链的帧缓冲区
    VkRenderPass loadRenderPass; // 主渲染通道, 目标是交换链的帧缓冲区
    VKSwapchain_t swapchain; // 交换链
    uint32_t frameIndex; // 当前帧索引

	bool inCommandRecording; // 当前帧命令缓冲区是否正在记录
	bool inRenderPass; // 当前帧渲染通道是否开始
	bool inRenderFrame; // 当前是否在帧begin和end之间
	int renderPassStage; // 当前渲染通道阶段
	
    VkSemaphore imageAvailableSemaphores[NUM_INFLIGHT_FRAME]; // 交换链回显图像同步的信号量
    VkSemaphore renderFinishedSemaphores[NUM_INFLIGHT_FRAME]; // 当前帧队列渲染完成同步的信号量
    VkFence inFlightFences[NUM_INFLIGHT_FRAME]; // 飞行帧的CPU同步栅栏
    VKQUEUE(VKDescriptorPool_t) descriptorPools[NUM_INFLIGHT_FRAME];

    VKCommandPool_t commandPool; // 渲染命令池
	VkPipelineCache pipelineCache; // 渲染管线缓存

	VKLIST(VkShaderModule) shaders; // 着色器列表(只能追加, 不能删除, 由上下文在销毁时清理)
	VKPTRLIST(VKPipelineLayout_t) pipelineLayouts; // 渲染管线布局列表(只能追加, 不能删除, 由上下文在销毁时清理)
	VKPTRLIST(VKPipeline_t) pipelines; // 渲染管线列表(只能追加, 不能删除, 由上下文在销毁时清理)

	VKPOOL(VKImage_t) images; // 图像(包括图像视图, 图像内存)列表(可插入, 可删除, 由上下文在销毁时清理)
	VKPOOL(VKBuffer_t) buffers; // 缓冲区(内存)列表(可插入, 可删除, 由上下文在销毁时清理)
	VKPOOL(VKQueryPool_t) querys; // 查询池列表(可插入, 可删除, 由上下文在销毁时清理)

	VKLIST(VkSampler) samplers; // 图像采样器列表(只能追加, 不能删除, 由上下文在销毁时清理)
	VKLIST(VkFence) fences; // 同步栅栏列表(只能追加, 不能删除, 由上下文在销毁时清理)
	VKPTRLIST(VKCommandPool_t) commandPools; // 命令池列表(只能追加, 不能删除, 由上下文在销毁时清理)
	VKLIST(VkRenderPass) renderPasses; // 渲染通道列表(只能追加, 不能删除, 由上下文在销毁时清理)

	VKPTRLIST(VKFramebuffer_t) framebuffers; // 帧缓冲区列表(只能追加, 不能删除, 由上下文在销毁时清理)

    bool initialized; // 是否初始化
} VKContext_t;

extern VKContext_t vk_context;

// Context
void VK_PreInit();
void VK_Init();
void VK_Destroy();
VkAllocationCallbacks * VK_GetAllocator();
void VK_SetWindowNativeID(void *window);
void VK_SetWindowSize(int width, int height);

// Frame
int VK_BeginFrame();
void VK_EndFrame();
int VK_SwapBuffers();
void VK_RecreateSwapchain();

// Misc
void VK_WaitIdle();
void VK_BeginRenderPass();
void VK_EndRenderPass();
void VK_ClearSwapchainAttachments();
void VK_ResetQueryPool(VKQueryPool_t *queryPool);
void VK_CreatePipelineCache(const void *data, int size);
int VK_GetPipelineCacheData(void **data);
void VK_WaitFence(VkFence fence);
void VK_WaitFrameFence();
bool VK_IsExtensionSupported(const char *name);

// debug
void VK_SetObjectName(int objType, uint64_t handle, const char* name);
void VK_DebugObjects();

// Allocate handler
VKHandle_t VK_AllocPipeline(VKPipeline_t **pipeline);VKHandle_t VK_AllocDescriptorSet(VkDescriptorSet **set, VkDescriptorPool *pool, int numUniforms, int numSamplers);
VKHandle_t VK_AllocSampler(VkSampler **sampler);
VKHandle_t VK_AllocBuffer(VKBuffer_t **buffer);
VKHandle_t VK_AllocImage(VKImage_t **image);
VKHandle_t VK_AllocRenderPass(VkRenderPass **renderPass);
VKHandle_t VK_AllocQueryPool(VKQueryPool_t **queryPool);
VKHandle_t VK_AllocShader(VkShaderModule **shaderModule);
VKHandle_t VK_AllocPipelineLayout(VKPipelineLayout_t **pipelineLayout);
VKHandle_t VK_AllocFence(VkFence **fence);
VKHandle_t VK_AllocCommandPool(VKCommandPool_t **commandPool);

// Free slot
void VK_FreeBuffer(VKHandle_t handle);
void VK_FreeImage(VKHandle_t handle);
void VK_FreeQueryPool(VKHandle_t handle);
void VK_FreeImageObject(VKImage_t *image);
void VK_FreeBufferObject(VKBuffer_t *buffer);

// Destroy
void VK_DestroyBuffer(VKBuffer_t *buffer);

// Pool handler
VKHandle_t VK_FindImageHandle(VKImage_t *image);
void VK_SwapBufferHandle(VKHandle_t a, VKHandle_t b);
void VK_SwapImageHandle(VKHandle_t a, VKHandle_t b);
VKHandle_t VK_FindBufferHandle(VKBuffer_t *buffer);

// Create
void VK_CreateCommandPool(VKCommandPool_t *commandPool, int usage, bool reset, bool createFence);
void VK_CreateImage(VKImage_t *res, uint32_t width, uint32_t height, int format, bool cubeMap, int layers, int mipmaps, int usage);
void VK_CreateBuffer(VKBuffer_t *ret, int usage, uint32_t size, bool map, bool flush);
void VK_CreateVertexBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush);
void VK_CreateIndexBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush);
void VK_CreateUniformBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush);
void VK_CreateTransferSrcBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush);
void VK_CreateTransferDstBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush);
VkImageView VK_CreateImageView(VkImage image, int format, int imageViewType, int viewAspectFlags, int mipLevels, int numLayers);
void VK_CreateSampler(VkSampler *sampler, bool linear, int repeatS, int repeatT, int repeatR, bool depthCmp, float anisotropy, float lodMax);
VkShaderModule VK_CreateShaderModule(const uint8_t *data, int length);
VKHandle_t VK_CreateDescriptorSet(VkDescriptorSet **descriptorSet, VkDescriptorPool *descriptorPool, VkDescriptorSetLayout layouts[], int count, int numUniforms, int numSamplers);
void VK_CreateQueryPool(VKQueryPool_t *query, int queryType, int num);

// Image
int VK_AllocImageBuffer(VKImage_t *image, VKBuffer_t *buffer, int size, const uint8_t *data, int offset, void *ptr);
void VK_WriteImageBuffer(VKImage_t *image, VKBuffer_t *buffer, int offset, int offsets[], int sizes[], VkCommandBuffer cmd);
void VK_CopyBufferToImage(VKImage_t *image, const VKBuffer_t *buffer, int offset, int mipLevels, int sizes[], int offsets[], VkCommandBuffer cmd);
void VK_CopyColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY);
void VK_CopyColorAttachmentImageToBuffer(VKBuffer_t *buffer, int x, int y, int width, int height, int offset, int cols, int rows, VkCommandBuffer cmd);
void VK_BlitColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight, bool flipY);

// Buffer
void * VK_MapBuffer(VKBuffer_t *buffer, int offset, int size);
void VK_UnmapBuffer(VKBuffer_t *buffer);
void VK_FlushBuffer(VKBuffer_t *buffer, int offset, int size);
void VK_InvalidateBuffer(VKBuffer_t *buffer, int offset, int size);
void VK_CopyBufferToBuffer(const VKBuffer_t *dstBuffer, const VKBuffer_t *buffer, int dstOffset, int offset, int size, VkCommandBuffer cmd);
void VK_WriteBuffer(VKBuffer_t *buffer, const void *src, int offset, int size, void *ptr);
void VK_ReadBuffer(VKBuffer_t *buffer, void *dst, int offset, int size, void *ptr);

// Query
void VK_GetQueryResult(VKQueryPool_t *query, int offset, int num, uint64_t *ret);

// Command
void VK_Viewport(int x, int y, int width, int height, float minZ, float maxZ);
void VK_Scissor(int x, int y, int width, int height);
void VK_LineWidth(float width);
void VK_DepthBounds(float minz, float maxz);
void VK_DrawArrays(int first, int count);
void VK_DrawElements(int count);
void VK_DrawElementsEXT(const VKBuffer_t *buffer, int count, int type, int offset);
void VK_PolygonOffset(float factor, float units);
void VK_StencilMaskSeparate(int face, unsigned int mask);
void VK_StencilMask(unsigned int mask);
void VK_StencilFuncSeparate(int face, int ref, int mask);
void VK_StencilFunc(int ref, int mask);
int VK_BeginQuery(VKQueryPool_t *query, int offset, bool precise);
void VK_EndQuery(VKQueryPool_t *query, int offset);
void VK_CopyQueryResult(VKQueryPool_t *query, int offset, int num, VKBuffer_t *buffer, int bufferOffset);
void VK_Clear(int mask, float r, float g, float b, float a, float depth, int stencil);
void VK_ClearColor(float r, float g, float b, float a);
void VK_ClearDepth(float depth);
void VK_ClearStencil(int stencil);
void VK_ClearDepthStencil(float depth, int stencil);

#endif //_VULKAN_VKCONTEXT_H
