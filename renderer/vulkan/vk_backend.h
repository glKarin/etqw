#ifndef _VULKAN_VKBACKEND_H
#define _VULKAN_VKBACKEND_H

#define VKBINDING_MAX_TRXTURE_UNITS 8 // 最大使用纹理数

#define ASSERT_PROGRAM assert(vk_backend.currentProgram);

#define globalUBO (vk_backend.ubo)

#if 1
#define VKDEBUG_SET_NAME(type, obj, ...) R_VK_SetObjectName(type, obj, __VA_ARGS__)
#else
#define VKDEBUG_SET_NAME(type, obj, ...)
#endif

/**
 * Uniform描述符类型
 */
typedef enum VKUboTypeMask_e
{
	UTM_VERTEX = BIT(0),
	UTM_FRAGMENT = BIT(1),
	UTM_IMAGE = BIT(2),
} VKUboTypeMask_t;

/**
 * 图像单元类型
 */
typedef enum VKTextureUnitType_e
{
	TUT_2D = 0,
	TUT_CUBEMAP,
	TUT_2D_ARRAY,
	TUT_TOTAL,
} VKTextureUnitType_t;

/**
 * 动态属性
 */
typedef struct VKDynamicState_s
{
	int renderWidth;
	int renderHeight;
	int viewport[4];
	int scissor[4];
	float lineWidth;
	float clearColor[4];
	float clearDepth;
	int clearStencil;
	float polygonFactor;
	float polygonUnits;
	int cullFace;
	unsigned int stencilMask[2];
	int stencilReference[2];
	unsigned int stencilWriteMask[2];
	float depthRanges[2];
} VKDynamicState_t;

/**
 * Vulkan全局后端
 */
typedef struct VKBackEnd_s
{
	const VKImage_t *tmu[TUT_TOTAL][VKBINDING_MAX_TRXTURE_UNITS]; // 当前绑定的每种类型的图像
	int currenttmu; // 当前tmu索引
	VKShaderProgram_t *currentProgram; // 当前绑定的着色器
	VKPipeline_t *currentPipeline; // 当前绑定的渲染管线

	globalStateSet_t state; // 当前渲染状态

	VKDynamicState_t dynamicState; // 当前Vulkan动态属性

	VKMAP(VkSampler, VKSamplerKey_t) samplerPool; // 采样器池
	VKQuery_t query; // 查询对象
	VkRenderPass currentRenderPass;

	globalUniformBufferObject_t ubo; // 当前Uniform变量
	int frameCount; // 帧计数
	globalUniformBufferObjectState_t uboState; // 当前Uniform缓存状态计数
	VkDescriptorSet currentDescriptorSet; // 当前绑定的描述符集

	VKLIST(VKHandle_t) purgeBuffers[NUM_INFLIGHT_FRAME]; // 延时删除缓冲区列表(实际删除时清空)
	VKLIST(VKHandle_t) purgeImages[NUM_INFLIGHT_FRAME]; // 延时删除图像列表(实际删除时清空)
	VKLIST(VKShaderProgram_t *) programs; // 全部渲染管线

#ifdef _SHADOW_MAPPING
	VkRenderPass shadowMappingRenderPass;
#endif

	bool isMirror;
	bool initialized; // 是否初始化
} VKBackEnd_t;
//static_assert(offsetof(VKBackEnd_t, ubo) + offsetof(globalUniformBufferObject_t, alphaTest) + 4 + 4 == offsetof(VKBackEnd_t, uboState));

extern VKBackEnd_t vk_backend;

void R_VK_Init();
void R_VK_Shutdown();
void R_VK_DestroyBuffer(VKHandle_t handle);
void R_VK_CreatePipelineLayout(VKShaderProgram_t *program);

void R_VK_UseProgram(VKShaderProgram_t *program);

void R_VK_Uniform4f(int location, float x, float y, float z, float w);
void R_VK_Uniform4fv(int location, const vec4 arr);
void R_VK_Uniform1f(int location, float x);
void R_VK_UniformMatrix4fv(int location, const mat4 mat);
void R_VK_UniformMatrix4fv(int location, const float mat[4][4]);
void R_VK_Uniform1i(int location, int x);
void R_VK_Uniform2fv(int location, const vec2 arr);
void R_VK_Uniform3fv(int location, const vec3 arr);
void R_VK_UniformMatrix4fv(int location, const mat4 mat, int n);

void R_VK_TransformUniform(int dst, int src, void *to, int size);
void R_VK_TransformUniform1i(int dst, int src, void *to);
void R_VK_TransformUniform1f(int dst, int src, void *to);
void R_VK_TransformUniform4f(int dst, int src, void *to);
void R_VK_TransformUniformMatrix4f(int dst, int src, void *to);

void R_VK_Bind(const VKImage_t *image);
void R_VK_BindType(int type, const VKImage_t *image);
void R_VK_SelectTexture(int unit);
void R_VK_BindTexture(int unit, const VKImage_t *image);
void R_VK_BindTextureType(int unit, int tut, const VKImage_t *image);
void R_VK_CopyColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight);
void R_VK_CopyDepthStencilAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight, int mask);

#define VKSTATE_STAGE_OFFSET(x) ( IS_SECOND_ST(x) ? offsetof(globalStateSet_t, st2) : offsetof(globalStateSet_t, st1) )
#define VKSTATE_STAGE(st, x) (*((globalState_t *)(((char *)&(st)) + VKSTATE_STAGE_OFFSET(x))))

#define VKSTATE1_IS_TRUE(x) (vk_backend.state.st1 & (x))
#define VKSTATE2_IS_TRUE(x) (vk_backend.state.st2 & (x))

void R_VK_ClearColor(float r, float g, float b, float a);
void R_VK_ClearDepth(float depth);
void R_VK_ClearStencil(int stencil);
void R_VK_EnableDepthTest(bool enabled);
void R_VK_EnableStencilTest(bool enabled);
void R_VK_EnableBlend(bool enabled);
void R_VK_EnableScissorTest(bool enabled);
void R_VK_EnableCullFace(bool enabled);
void R_VK_EnablePolygonOffset(bool enabled);
bool R_VK_IsDepthTestEnabled();
bool R_VK_IsStencilTestEnabled();
bool R_VK_IsBlendEnabled();
bool R_VK_IsScissorTestEnabled();
bool R_VK_IsCullFaceEnabled();
bool R_VK_IsPolygonOffsetEnabled();
void R_VK_DepthMask(bool write);
void R_VK_ColorMask(bool r, bool g, bool b, bool a);
void R_VK_CullFace(int face);
void R_VK_PolygonMode(int mode);
void R_VK_Begin(int mode);
void R_VK_FrontFace(int mode);
void R_VK_DepthFunc(int func);
void R_VK_BlendFunc(int src, int dst);
void R_VK_StencilOp(int sfail, int dpfail, int dppass);
void R_VK_StencilOpSeparate(int face, int sfail, int dpfail, int dppass);
void R_VK_Clear(bool color, bool depth, bool stencil);
void R_VK_StencilFunc(int func, int ref, unsigned int mask);
void R_VK_StencilFuncSeparate(int face, int func, int ref, unsigned int mask);
void R_VK_StencilMask(unsigned int mask);
void R_VK_StencilMaskSeparate(int face, unsigned int mask);
void R_VK_PolygonOffset(float factor, float units);
void R_VK_DrawArrays(int mode, int first, int count);
void R_VK_Color4f(float r, float g, float b, float a);
void R_VK_Color4fv(float color[4]);
void R_VK_UseVertexColor(bool enable);
void R_VK_Viewport(int x, int y, int w, int h);
void R_VK_Scissor(int x, int y, int w, int h);
void R_VK_LineWidth(float width);
void R_VK_DepthRange(float minZ, float maxZ);
void R_VK_SetRenderSize(int w, int h);
void R_VK_SetRenderPass(VkRenderPass renderPass, int w, int h);

void R_VK_UpdateUniformBufferObject();
void R_VK_BeginFrame();
void R_VK_EndFrame();
void R_VK_UploadImageData(VKImage_t *image, int totalSize, const byte *totalData, int offsets[], int sizes[]);
void R_VK_UploadImageSubData(VKImage_t *image, int totalSize, const byte *totalData, int level, int offsets[], int sizes[]);
void R_VK_UploadBufferData(VKBuffer_t *dstBuffer, int size, const void *data, int offset);
void R_VK_AllocVertexBuffer(const void *data, int size, VKBufferMap_t *map);
void R_VK_AllocIndexBuffer(const void *data, int size, VKBufferMap_t *map);
void R_VK_BindProgram();
void R_VK_DestroyImage(VKHandle_t handle);
void R_VK_RecreateImage(VKHandle_t texnum, VKImage_t **image);
void R_VK_SetObjectName(int objType, uint64_t handle, const char *name, ...);
void R_VK_CmdDraw(int vertexCount, int firstVertex);
void R_VK_CmdDrawInstance(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
void R_VK_CmdDrawIndexed(int indexCount, int firstIndex, int vertexOffset);
void R_VK_CmdDrawIndexedInstance(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
void R_VK_CmdBindIndexBuffer(VkBuffer buffer, int offset);
void R_VK_CmdBindVertexBuffer(VkBuffer pBuffer, VkDeviceSize offset);
void R_VK_CmdBindVertexBuffers(int bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);

#ifdef _SHADOW_MAPPING
VkRenderPass R_VK_CreateShadowMappingRenderPass( void );
#endif

#endif //_VULKAN_VKBACKEND_H
