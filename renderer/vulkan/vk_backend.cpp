#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#define VKPIPELINE_CACHE_FILE "vulkan_pipeline.cache.bin"

#define GLOBAL_UBO_PTR(location) ((char *)&globalUBO + (location))
#define GLOBAL_UBO_PTR_T(T, location) ((T *)GLOBAL_UBO_PTR(location))
#define GLOBAL_UBO_PTR_FLOAT(location) GLOBAL_UBO_PTR_T(float, location)
#define GLOBAL_UBO_PTR_INT(location) GLOBAL_UBO_PTR_T(int, location)

#define GLOBAL_UBO_STATE_PTR(location) (((char *)&globalUBOState) + (location))
#define GLOBAL_UBO_STATE_VALUE(location) (*((int *)GLOBAL_UBO_STATE_PTR(location)))
#define GLOBAL_UBO_STATE_MOD(location) GLOBAL_UBO_STATE_VALUE(location)++

#define SHADER_UBO_PTR(ptr, location) ((char *)(ptr) + (location))
#define SHADER_UBO_PTR_T(T, ptr, location) ((T *)SHADER_UBO_PTR(ptr, location))
#define SHADER_UBO_PTR_FLOAT(ptr, location) SHADER_UBO_PTR_T(float, ptr, location)
#define SHADER_UBO_PTR_INT(ptr, location) SHADER_UBO_PTR_T(int, ptr, location)

#if IDVK_CONTAINER_SIZE_CONFIG

#define IDVK_SAMPLER_INIT_SIZE ( r_vkBackendSamplerSize.GetInteger() )
#define IDVK_SAMPLER_GRANULARITY ( r_vkBackendSamplerGranularity.GetInteger() )

#define IDVK_FREE_BUFFER_INIT_SIZE ( r_vkBackendFreeBufferSize.GetInteger() )
#define IDVK_FREE_BUFFER_GRANULARITY ( r_vkBackendFreeBufferGranularity.GetInteger() )

#define IDVK_PURGE_IMAGE_INIT_SIZE ( r_vkBackendPurgeImageSize.GetInteger() )
#define IDVK_PURGE_IMAGE_GRANULARITY ( r_vkBackendPurgeImageSize.GetInteger() )

#define IDVK_PILELINE_LAYOUT_INIT_SIZE ( r_vkBackendPipelineLayoutSize.GetInteger() )
#define IDVK_PILELINE_LAYOUT_GRANULARITY ( r_vkBackendPipelineLayoutGranularity.GetInteger() )

#else

#define IDVK_SAMPLER_INIT_SIZE 128
#define IDVK_SAMPLER_GRANULARITY 128

#define IDVK_FREE_BUFFER_INIT_SIZE 1024
#define IDVK_FREE_BUFFER_GRANULARITY 1024

#define IDVK_PURGE_IMAGE_INIT_SIZE 512
#define IDVK_PURGE_IMAGE_GRANULARITY 512

#define IDVK_PILELINE_LAYOUT_INIT_SIZE 64
#define IDVK_PILELINE_LAYOUT_GRANULARITY 64

#endif

VKBackEnd_t vk_backend;

#define globalUBOState (vk_backend.uboState)

#define VKSTATE1_CLEAR(x) vk_backend.state.st1 &= ~(globalState_t)(x);
#define VKSTATE2_CLEAR(x) vk_backend.state.st2 &= ~(globalState_t)(x);

static void R_VK_LoadPipelineCache(void)
{
	if (!r_vkUsePipelineCache.GetBool())
		return;

	uint8_t *data = NULL;
	int length = fileSystem->ReadFile(VKPIPELINE_CACHE_FILE, (void **)&data, NULL);

	if(length < 0)
		length = 0;

	VK_CreatePipelineCache(data, length);

	if(length > 0)
	{
		common->Printf("[Vulkan]: Using pipeline cache: %d\n", length);
	}

	if (data)
		fileSystem->FreeFile(data);
}

static void R_VK_SavePipelineCache(void)
{
	if (!r_vkUsePipelineCache.GetBool())
		return;

	uint8_t *data = NULL;
	int length = VK_GetPipelineCacheData((void **)&data);
	if(length > 0)
	{
		common->Printf("[Vulkan]: Dump pipeline cache: %d\n", length);
		fileSystem->WriteFile(VKPIPELINE_CACHE_FILE, data, length);
	}
	free(data);
}

void R_VK_Uniform1f(int location, float value)
{
	assert(location >= 0);
	assert(sizeof(globalUniformBufferObject_t) >= location + 4 );
	*GLOBAL_UBO_PTR_FLOAT(location) = value;
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_Uniform4f(int location, float x, float y, float z, float w)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 16 );
	assert(location >= 0);
	float *ptr = GLOBAL_UBO_PTR_FLOAT(location);
	ptr[0] = x;
	ptr[1] = y;
	ptr[2] = z;
	ptr[3] = w;
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_Uniform4fv(int location, const vec4 arr)
{
	//memcpy(GLOBAL_UBO_PTR(location), arr, sizeof(float) * 4);
	assert(sizeof(globalUniformBufferObject_t) >= location + 16 );
	assert(location >= 0);
	float *ptr = GLOBAL_UBO_PTR_FLOAT(location);
	ptr[0] = arr[0];
	ptr[1] = arr[1];
	ptr[2] = arr[2];
	ptr[3] = arr[3];
	GLOBAL_UBO_STATE_MOD(location);
}

static bool fltcmp(float a, float b)
{
	return fabs(a - b) <= 1e-6;
}

static bool mat4cmp(mat4 a, mat4 b)
{
	for (int i = 0; i < 16; ++i)
	{
		if (!fltcmp(a[i], b[i]))
			return false;
	}
	return true;
}

static void mat4print(mat4 a)
{
	for (int i = 0; i < 16; ++i)
	{
		Sys_Printf("%.6f ", a[i]);
	}
	Sys_Printf("\n");
}

void R_VK_UniformMatrix4fv(int location, const mat4 mat)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 64 );
	assert(location >= 0);
	memcpy(GLOBAL_UBO_PTR(location), mat, sizeof(float) * 16);
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_UniformMatrix4fv(int location, const float mat[4][4])
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 64 );
	assert(location >= 0);
	memcpy(GLOBAL_UBO_PTR(location), mat, sizeof(float) * 16);
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_Uniform1i(int location, int value)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 4 );
	assert(location >= 0);
	*GLOBAL_UBO_PTR_INT(location) = value;
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_Uniform2fv(int location, const vec2 arr)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 16 );
	assert(location >= 0);
	float *ptr = GLOBAL_UBO_PTR_FLOAT(location);
	ptr[0] = arr[0];
	ptr[1] = arr[1];
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_Uniform3fv(int location, const vec3 arr)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 16 );
	assert(location >= 0);
	float *ptr = GLOBAL_UBO_PTR_FLOAT(location);
	ptr[0] = arr[0];
	ptr[1] = arr[1];
	ptr[2] = arr[2];
	GLOBAL_UBO_STATE_MOD(location);
}

void R_VK_UniformMatrix4fv(int location, const mat4 mat, int n)
{
	assert(sizeof(globalUniformBufferObject_t) >= location + 64*n );
	assert(location >= 0);
	memcpy(GLOBAL_UBO_PTR(location), mat, sizeof(float) * 16 * n);
	GLOBAL_UBO_STATE_MOD(location);
}

#define SHADER_UBO_STATE_PTR(location) (((char *)&vk_backend.currentProgram->uboState) + (location))
#define SHADER_UBO_STATE_VALUE(location) (*((int *)SHADER_UBO_STATE_PTR(location)))

#if 0
#define SHADER_UBO_STATE_HAS_UPDATE(location) \
	{ \
		if(vk_backend.frameCount != vk_backend.currentProgram->frameCount && vk_backend.frameCount != vk_backend.currentProgram->frameCount) \
			Sys_Printf("%s update:frame(%d != %d) || counter(%d != %d)\n", __func__, vk_backend.frameCount, vk_backend.currentProgram->frameCount, GLOBAL_UBO_STATE_VALUE(location), SHADER_UBO_STATE_VALUE(location)); \
		else if(vk_backend.frameCount != vk_backend.currentProgram->frameCount) \
			Sys_Printf("%s update: frame(%d != %d)\n", __func__, vk_backend.frameCount, vk_backend.currentProgram->frameCount); \
		else if(vk_backend.frameCount != vk_backend.currentProgram->frameCount) \
			Sys_Printf("%s update: counter(%d != %d)\n", __func__, GLOBAL_UBO_STATE_VALUE(location), SHADER_UBO_STATE_VALUE(location)); \
	}
#define SHADER_UBO_STATE_NOT_UPDATE else { Sys_Printf("%s not update\n", __func__); }
#else
#define SHADER_UBO_STATE_HAS_UPDATE(location)
#define SHADER_UBO_STATE_NOT_UPDATE
#endif

#define SHADER_UBO_STATE_IS_DIRTY(location) (GLOBAL_UBO_STATE_VALUE(location) != SHADER_UBO_STATE_VALUE(location) || vk_backend.frameCount != vk_backend.currentProgram->frameCount)
#define SHADER_UBO_STATE_CLEAR_DIRTY(location) { SHADER_UBO_STATE_HAS_UPDATE(location) SHADER_UBO_STATE_VALUE(location) = GLOBAL_UBO_STATE_VALUE(location); vk_backend.currentProgram->uboUpdate = true; }

typedef struct vec4_s
{
	vec4 v;
} vec4_t;

typedef struct mat4_s
{
	mat4 m;
} mat4_t;

#define MAT4CPY(dst, src) *((mat4_t *)(dst)) = *((mat4_t *)(src))
#define VEC4CPY(dst, src) *((vec4_t *)(dst)) = *((vec4_t *)(src))

#if 0
void R_VK_Uniform1f2(int location, int st_location, float value)
{
	*GLOBAL_UBO_PTR_FLOAT(location) = value;
	GLOBAL_UBO_STATE_MOD(st_location);
}

void R_VK_Uniform4f2(int location, int st_location, float x, float y, float z, float w)
{
	float *ptr = GLOBAL_UBO_PTR_FLOAT(location);
	ptr[0] = x;
	ptr[1] = y;
	ptr[2] = z;
	ptr[3] = w;
	GLOBAL_UBO_STATE_MOD(st_location);
}

void R_VK_Uniform4fv2(int location, int st_location, const vec4 arr)
{
	memcpy(GLOBAL_UBO_PTR(location), arr, sizeof(float) * 4);
	GLOBAL_UBO_STATE_MOD(st_location);
}

void R_VK_UniformMatrix4fv2(int location, int st_location, const mat4 mat)
{
	memcpy(GLOBAL_UBO_PTR(location), mat, sizeof(float) * 16);
	GLOBAL_UBO_STATE_MOD(st_location);
}

void R_VK_UniformMatrix4fv2(int location, int st_location, const float mat[4][4])
{
	memcpy(GLOBAL_UBO_PTR(location), mat, sizeof(float) * 16);
	GLOBAL_UBO_STATE_MOD(st_location);
}

void R_VK_TransformUniform2(int dst, int src, int st_location, void *to, int size)
{
	if (SHADER_UBO_STATE_IS_DIRTY(st_location))
	{
		memcpy(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src), size);
		SHADER_UBO_STATE_CLEAR_DIRTY(st_location);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform1i2(int dst, int src, int st_location, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(st_location))
	{
		*SHADER_UBO_PTR_INT(to, dst) = *GLOBAL_UBO_PTR_INT(src);
		SHADER_UBO_STATE_CLEAR_DIRTY(st_location);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform1f2(int dst, int src, int st_location, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(st_location))
	{
		*SHADER_UBO_PTR_FLOAT(to, dst) = *GLOBAL_UBO_PTR_FLOAT(src);
		SHADER_UBO_STATE_CLEAR_DIRTY(st_location);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform4f2(int dst, int src, int st_location, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(st_location))
	{
		VEC4CPY(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src));
		SHADER_UBO_STATE_CLEAR_DIRTY(st_location);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniformMatrix4f2(int dst, int src, int st_location, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(st_location))
	{
		MAT4CPY(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src));
		SHADER_UBO_STATE_CLEAR_DIRTY(st_location);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}
#endif

void R_VK_TransformUniform(int dst, int src, void *to, int size)
{
	if (SHADER_UBO_STATE_IS_DIRTY(src))
	{
		memcpy(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src), size);
		SHADER_UBO_STATE_CLEAR_DIRTY(src);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform1i(int dst, int src, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(src))
	{
		*SHADER_UBO_PTR_INT(to, dst) = *GLOBAL_UBO_PTR_INT(src);
		SHADER_UBO_STATE_CLEAR_DIRTY(src);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform1f(int dst, int src, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(src))
	{
		*SHADER_UBO_PTR_FLOAT(to, dst) = *GLOBAL_UBO_PTR_FLOAT(src);
		SHADER_UBO_STATE_CLEAR_DIRTY(src);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniform4f(int dst, int src, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(src))
	{
		VEC4CPY(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src));
		SHADER_UBO_STATE_CLEAR_DIRTY(src);
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

void R_VK_TransformUniformMatrix4f(int dst, int src, void *to)
{
	if (SHADER_UBO_STATE_IS_DIRTY(src))
	{
		MAT4CPY(SHADER_UBO_PTR(to, dst), GLOBAL_UBO_PTR(src));
		SHADER_UBO_STATE_CLEAR_DIRTY(src);
	}
	else
	{
		if (!mat4cmp((float *)SHADER_UBO_PTR(to, dst), (float *)GLOBAL_UBO_PTR(src)))
		{
			Sys_Printf("XXX %s %p|%p, %d|%d, %d=%d, %d=%d\n", vk_backend.currentProgram->name, GLOBAL_UBO_PTR(src), SHADER_UBO_PTR(to, dst), vk_backend.currentProgram->frameCount, vk_backend.frameCount, src, dst, GLOBAL_UBO_STATE_VALUE(src), SHADER_UBO_STATE_VALUE(src));
			mat4print((float *)GLOBAL_UBO_PTR(src));
			mat4print((float *)SHADER_UBO_PTR(to, dst));
		}
		//if (src == offsetof(globalUniformBufferObject_t, modelViewProjectionMatrix))
	}
	SHADER_UBO_STATE_NOT_UPDATE
}

/**
 * 创建渲染管线
 * @param vertex 顶点着色器模块
 * @param fragment 片段着色器模块
 */
void R_VK_CreatePipeline(VKShaderProgram_t *program)
{
    VkResult result;

	VkCompareOp depthCompareOp = R_VK_GetDepthCompareOp(program->currentState.st1);
	VkPrimitiveTopology primitiveTopology = R_VK_GetPrimitiveTopology(program->currentState.st1);
	VkBool32 depthTestEnable = VKBACKEND_STATE1_BOOL(ST_DEPTH_TEST_BIT);
	VkBool32 stencilTestEnable = VKBACKEND_STATE1_BOOL(ST_STENCIL_TEST_BIT);
	VkBool32 blendEnable = VKBACKEND_STATE1_BOOL(ST_BLEND_BIT);
	VkBool32 depthBiasEnable = VKBACKEND_STATE1_BOOL(ST_POLYGON_OFFSET_BIT);
	VkBlendFactor srcBlendFactor = R_VK_GetSrcBlendFactor(program->currentState.st1);
	VkBlendFactor dstBlendFactor = R_VK_GetDstBlendFactor(program->currentState.st1);
	VkBool32 depthWriteEnable = R_VK_GetDepthWriteEnable(program->currentState.st1);
	VkColorComponentFlags colorWriteMask = R_VK_GetColorWriteMask(program->currentState.st1);
	VkPolygonMode polygonMode = R_VK_GetPolygonMode(program->currentState.st1);
	VkCullModeFlags cullMode = R_VK_GetCullMode(program->currentState.st1, vk_backend.isMirror);
	VkFrontFace frontFace = R_VK_GetFrontFace(program->currentState.st1);

	VkStencilOp frontFailOp = R_VK_GetStencilFrontFailOp(program->currentState.st2);
	VkStencilOp frontPassOp = R_VK_GetStencilFrontPassOp(program->currentState.st2);
	VkStencilOp frontDepthFailOp = R_VK_GetStencilFrontDepthFailOp(program->currentState.st2);
	VkCompareOp frontCompareOp = R_VK_GetStencilFrontCompareOp(program->currentState.st2);
	VkStencilOp backFailOp = R_VK_GetStencilBackFailOp(program->currentState.st2);
	VkStencilOp backPassOp = R_VK_GetStencilBackPassOp(program->currentState.st2);
	VkStencilOp backDepthFailOp = R_VK_GetStencilBackDepthFailOp(program->currentState.st2);
	VkCompareOp backCompareOp = R_VK_GetStencilBackCompareOp(program->currentState.st2);

	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo;
	VKZERO(vertexShaderCreateInfo);
    R_CreateVertexShader(&vertexShaderCreateInfo, program->pipelineLayout->vertexShader);

	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo;
	VKZERO(fragmentShaderCreateInfo);
    R_CreateFragmentShader(&fragmentShaderCreateInfo, program->pipelineLayout->fragmentShader);

    VkPipelineShaderStageCreateInfo createInfos[] = {
            vertexShaderCreateInfo,
            fragmentShaderCreateInfo,
    };

    // 配置顶点数组参数输入: 顶点着色器
	VkVertexInputBindingDescription vertexInputBindingDescription;
	VKZERO(vertexInputBindingDescription);
    vertexInputBindingDescription.binding = 0;
	if(program->vertexInputAttribute & VIA_VERTEX4_BIT)
		vertexInputBindingDescription.stride = sizeof(shadowCache_t);
	else
		vertexInputBindingDescription.stride = sizeof(idDrawVert);
    vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // 配置顶点数组参数输入绑定位置
    VkVertexInputAttributeDescription vertexInputAttributeDescriptions[VIA_TOTAL];
    int vertexInputAttributeCount = R_VK_MakeVertexInputAttributes(program->vertexInputAttribute, vertexInputBindingDescription.binding, vertexInputAttributeDescriptions);

	// 管线顶点输入参数
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
	VKZERO(vertexInputStateCreateInfo);
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputAttributeCount;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;

    // 管线图元拓扑参数
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
	VKZERO(inputAssemblyStateCreateInfo);
    inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology = primitiveTopology;
    inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	// 管线视口和裁剪参数
    VkRect2D scissor;
	VKZERO(scissor);
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = vk_context.swapchain.width;
    scissor.extent.height = vk_context.swapchain.height;
    VkViewport viewportState;
	VKZERO(viewportState);
    viewportState.x = 0.0f;
    viewportState.y = 0.0f;
    viewportState.width = (float)vk_context.swapchain.width;
    viewportState.height = (float)vk_context.swapchain.height;
    viewportState.minDepth = 0.0f;
    viewportState.maxDepth = 1.0f;
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
	VKZERO(viewportStateCreateInfo);
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewportState;

    // 光栅化参数
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
	VKZERO(rasterizationStateCreateInfo);
    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable;
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode = polygonMode;
    rasterizationStateCreateInfo.lineWidth = 1.0f;
    rasterizationStateCreateInfo.cullMode = cullMode;
    rasterizationStateCreateInfo.frontFace = frontFace;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

	// 管线多重采样参数
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
	VKZERO(multisampleStateCreateInfo);
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = NULL;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	// 管线深度模版测试参数
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
	VKZERO(depthStencilStateCreateInfo);
    depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
    depthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
    depthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
    depthStencilStateCreateInfo.minDepthBounds = 0.0f;
    depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
	depthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable;
	depthStencilStateCreateInfo.front.failOp = frontFailOp;
	depthStencilStateCreateInfo.front.passOp = frontPassOp;
	depthStencilStateCreateInfo.front.depthFailOp = frontDepthFailOp;
	depthStencilStateCreateInfo.front.compareOp = frontCompareOp;
	depthStencilStateCreateInfo.back.failOp = backFailOp;
	depthStencilStateCreateInfo.back.passOp = backPassOp;
	depthStencilStateCreateInfo.back.depthFailOp = backDepthFailOp;
	depthStencilStateCreateInfo.back.compareOp = backCompareOp;

	// 关联颜色混合参数
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	VKZERO(colorBlendAttachmentState);
    colorBlendAttachmentState.colorWriteMask = colorWriteMask;
    colorBlendAttachmentState.blendEnable = blendEnable;
    colorBlendAttachmentState.srcColorBlendFactor = srcBlendFactor;
    colorBlendAttachmentState.srcAlphaBlendFactor = srcBlendFactor;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.dstColorBlendFactor = dstBlendFactor;
    colorBlendAttachmentState.dstAlphaBlendFactor = dstBlendFactor;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
	VKZERO(colorBlendStateCreateInfo);
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
    colorBlendStateCreateInfo.attachmentCount = 1;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	// 管线动态参数
    const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    	VK_DYNAMIC_STATE_LINE_WIDTH,
    	VK_DYNAMIC_STATE_DEPTH_BIAS,
    	VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
    	VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
    	VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	VKZERO(dynamicStateCreateInfo);
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = COUNT_OF(dynamicStates);
    dynamicStateCreateInfo.pDynamicStates = dynamicStates;

	// 创建渲染管线
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
	VKZERO(graphicsPipelineCreateInfo);
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.stageCount = COUNT_OF(createInfos);
    graphicsPipelineCreateInfo.pStages = createInfos;
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
    graphicsPipelineCreateInfo.renderPass = vk_context.renderPass;
    graphicsPipelineCreateInfo.layout = program->pipelineLayout->pipelineLayout;
    graphicsPipelineCreateInfo.subpass = 0;
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.basePipelineIndex = -1;

    VKPipeline_t *pipeline;
	VKHandle_t handle = VK_AllocPipeline(&pipeline);
    result = vkCreateGraphicsPipelines(vk_context.device, vk_context.pipelineCache, 1, &graphicsPipelineCreateInfo, NULL, &pipeline->pipeline);
	HANDLE_VK_ERROR_EXIT(result, "vkCreateGraphicsPipelines");

	program->pipeline = pipeline;
	program->pipeline->pipelineLayout = *program->pipelineLayout;
	program->currentState = vk_backend.state;

	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_PIPELINE, (uint64_t)pipeline->pipeline, "pipeline::%d::%s", handle, program->name);

	const char *stStr = R_VK_StateStr(program->currentState);
	Sys_Printf("Create new pipeline: %s, state=0x%llx|%llx = %s\n", program->name, program->currentState.st1, program->currentState.st2, stStr);
}

/**
 * 获取渲染管线(会自动根据渲染管线的固定状态变化时创建对应的渲染管线)
 */
static void R_VK_FindProgram(VKShaderProgram_t *program)
{
	VKMAP_FINDINDEX(VKPipeline_t, globalStateSet_t, program->pipelinePool, vk_backend.state, GLOBAL_ST_EQUALS);
	program->currentState = vk_backend.state;
	if (_vkc_index >= 0 && _vkc_index < program->pipelinePool.num)
	{
		program->pipeline = &program->pipelinePool.list[_vkc_index];
	}
	else
	{
		R_VK_CreatePipeline(program);
		VKMAP_ALLOC(VKPipeline_t, globalStateSet_t, program->pipelinePool);
		*_vkc_value = *program->pipeline;
		*_vkc_key = vk_backend.state;
	}
}

/**
 * 执行设置渲染管线可变的状态的命令
 */
static void R_VK_ExecuteVariableCmd()
{
	VK_Viewport(vk_backend.dynamicState.viewport[0], vk_backend.dynamicState.viewport[1], vk_backend.dynamicState.viewport[2], vk_backend.dynamicState.viewport[3], vk_backend.dynamicState.depthRanges[0], vk_backend.dynamicState.depthRanges[1]);
	if(VKSTATE1_BIT(vk_backend.state, ST_SCISSOR_TEST_BIT))
		VK_Scissor(vk_backend.dynamicState.scissor[0], vk_backend.dynamicState.scissor[1], vk_backend.dynamicState.scissor[2], vk_backend.dynamicState.scissor[3]);
	else
		VK_Scissor(0, 0, vk_context.swapchain.width, vk_context.swapchain.height);
	VK_LineWidth(vk_backend.dynamicState.lineWidth);
	VK_PolygonOffset(vk_backend.dynamicState.polygonFactor, vk_backend.dynamicState.polygonUnits);
	VK_StencilFuncSeparate(VK_STENCIL_FACE_FRONT_BIT, vk_backend.dynamicState.stencilReference[0], vk_backend.dynamicState.stencilMask[0]);
	VK_StencilFuncSeparate(VK_STENCIL_FACE_BACK_BIT, vk_backend.dynamicState.stencilReference[1], vk_backend.dynamicState.stencilMask[1]);
	VK_StencilMaskSeparate(VK_STENCIL_FACE_FRONT_BIT, vk_backend.dynamicState.stencilWriteMask[0]);
	VK_StencilMaskSeparate(VK_STENCIL_FACE_BACK_BIT, vk_backend.dynamicState.stencilWriteMask[1]);
}

/**
 * 绑定当前渲染管线
 */
static void R_VK_BindPipeline()
{
	vkCmdBindPipeline(vk_context.currentFrameCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_backend.currentProgram->pipeline->pipeline);
	//R_VK_ExecuteVariableCmd();
}

/**
 * 获取并绑定当前渲染管线(会自动根据渲染管线的固定状态变化时创建对应的渲染管线)
 */
static void R_VK_SyncPipelineState()
{
	ASSERT_PROGRAM;

	if (GLOBAL_ST_EQUALS(vk_backend.currentProgram->currentState, vk_backend.state) && vk_backend.currentPipeline && vk_backend.currentProgram->pipeline == vk_backend.currentPipeline) {
		return;
	}

	R_VK_FindProgram(vk_backend.currentProgram);

	if(vk_backend.currentProgram->pipeline != vk_backend.currentPipeline)
	{
		vk_backend.currentPipeline = vk_backend.currentProgram->pipeline;
		R_VK_BindPipeline();
	}

	//VKZERO(vk_backend.currentProgram->uboState);
}

/**
 * 设置当前渲染管线(现在不会立即绑定, 也不会立即创建渲染管线)
 */
void R_VK_UseProgram(VKShaderProgram_t *program)
{
	if (program)
	{
		if (vk_backend.currentProgram != program)
		{
			vk_backend.currentProgram = program;
			vk_backend.currentPipeline = NULL;
		}
	}
	else
	{
		vk_backend.currentProgram = NULL;
	}
}

void R_VK_BindProgram()
{
	if (vk_backend.currentProgram)
	{
		R_VK_BindPipeline();
	}
}

/**
 * 创建渲染管线布局
 */
void R_VK_CreatePipelineLayout(VKShaderProgram_t *program)
{
    VkResult result;

    VkShaderModule vertexModule = program->vertexShader;
    VkShaderModule fragmentModule = program->fragmentShader;
    VkDescriptorSetLayout descriptorSetLayout;
    VKPipelineLayout_t *pipelineLayout;
	VKHandle_t handle = VK_AllocPipelineLayout(&pipelineLayout);

	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo;
	VKZERO(vertexShaderCreateInfo);
    R_CreateVertexShader(&vertexShaderCreateInfo, vertexModule);

	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo;
	VKZERO(fragmentShaderCreateInfo);
    R_CreateFragmentShader(&fragmentShaderCreateInfo, fragmentModule);

    VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[VKBINDING_NUM];
	VKZERO(descriptorSetLayoutBindings);
	int numDescriptorSetLayoutBindings = 0;

	if (program->ubo_vertex)
	{
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].binding = 0;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorCount = 1;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].pImmutableSamplers = NULL;
		numDescriptorSetLayoutBindings++;
	}

	if (program->ubo_fragment)
	{
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].binding = 1;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorCount = 1;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].pImmutableSamplers = NULL;
		numDescriptorSetLayoutBindings++;
	}

	if (program->numTextureUnits > 0)
	{
		for (int i = 0; i < program->numTextureUnits; i++)
		{
			descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].binding = VKBINDING_TRXTURES_SLOT(i);
			descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorCount = 1;
			descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			descriptorSetLayoutBindings[numDescriptorSetLayoutBindings].pImmutableSamplers = NULL;
			numDescriptorSetLayoutBindings++;
		}
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	VKZERO(descriptorSetLayoutCreateInfo);
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = numDescriptorSetLayoutBindings;
    descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings;

    result = vkCreateDescriptorSetLayout(vk_context.device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateDescriptorSetLayout");
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, (uint64_t)descriptorSetLayout, "descriptorSetLayout::%d::%s", handle, program->name);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	VKZERO(pipelineLayoutCreateInfo);
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
    result = vkCreatePipelineLayout(vk_context.device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout->pipelineLayout);
    HANDLE_VK_ERROR_EXIT(result, "vkCreatePipelineLayout");

	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_PIPELINE_LAYOUT, (uint64_t)pipelineLayout->pipelineLayout, "pipelineLayout::%d::%s", handle, program->name);

    pipelineLayout->vertexShader = vertexModule;
    pipelineLayout->fragmentShader = fragmentModule;
    pipelineLayout->descriptorSetLayout = descriptorSetLayout;

    program->pipelineLayout = pipelineLayout;
}

/**
 * 初始化渲染后端
 */
static void R_VK_InitBackend()
{
	VKZERO(vk_backend);

	vk_backend.state = ST_DEFAULT_BITS;

	VKZERO(vk_backend.samplerPool);
	VKMAP_INIT(VkSampler, int, vk_backend.samplerPool, IDVK_SAMPLER_INIT_SIZE, IDVK_SAMPLER_GRANULARITY);
	VKLIST_INIT(VKShaderProgram_t *, vk_backend.programs, IDVK_PILELINE_LAYOUT_INIT_SIZE, IDVK_PILELINE_LAYOUT_GRANULARITY);

	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKLIST_INIT(VKHandle_t, vk_backend.purgeBuffers[i], IDVK_FREE_BUFFER_INIT_SIZE, IDVK_FREE_BUFFER_GRANULARITY);

		VKLIST_INIT(idImage, vk_backend.purgeImages[i], IDVK_PURGE_IMAGE_INIT_SIZE, IDVK_PURGE_IMAGE_GRANULARITY);
	}

	vk_backend.dynamicState.viewport[0] = 0;
	vk_backend.dynamicState.viewport[1] = 0;
	vk_backend.dynamicState.viewport[2] = vk_context.swapchain.width;
	vk_backend.dynamicState.viewport[3] = vk_context.swapchain.height;
	vk_backend.dynamicState.depthRanges[0] = 0.0f;
	vk_backend.dynamicState.depthRanges[1] = 1.0f;
	vk_backend.dynamicState.scissor[0] = 0;
	vk_backend.dynamicState.scissor[1] = 0;
	vk_backend.dynamicState.scissor[2] = vk_context.swapchain.width;
	vk_backend.dynamicState.scissor[3] = vk_context.swapchain.height;
	vk_backend.dynamicState.lineWidth = 1.0f;
	vk_backend.dynamicState.clearColor[0] = 0.0f;
	vk_backend.dynamicState.clearColor[1] = 0.0f;
	vk_backend.dynamicState.clearColor[2] = 0.0f;
	vk_backend.dynamicState.clearColor[3] = 0.0f;
	vk_backend.dynamicState.clearDepth = 1.0f;
	vk_backend.dynamicState.clearStencil = 0;
	vk_backend.dynamicState.polygonFactor = 0.0f;
	vk_backend.dynamicState.polygonUnits = 0.0f;
    vk_backend.dynamicState.cullFace = ST_CULL_NONE;
    vk_backend.dynamicState.stencilReference[0] = vk_backend.dynamicState.stencilReference[1] = 0;
    vk_backend.dynamicState.stencilMask[0] = vk_backend.dynamicState.stencilMask[1] = 0xFFFFFFFFu;
    vk_backend.dynamicState.stencilWriteMask[0] = vk_backend.dynamicState.stencilWriteMask[1] = 0xFFFFFFFFu;
}

static void R_VK_DebugObjects()
{
	if (r_vkSetDebugInfo.GetBool() && VK_IsExtensionSupported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
	{
		VK_DebugObjects();
	}
}

/**
 * 初始化渲染器
 */
void R_VK_Init()
{
	if (vk_backend.initialized)
		return;

	R_VK_InitBackend();
	R_VK_InitGlobalUBO();
	R_VK_InitBuffer();
	R_VK_LoadPipelineCache();
	R_VK_InitQuery();
	R_VK_DebugObjects();

	vk_backend.initialized = true;
}

/**
 * 终止渲染后端
 */
void R_VK_ShutdownBackend()
{
	if (!vk_backend.initialized)
		return;

	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		VKARRAY_FREE(uint64_t, vk_backend.query.queryResult[i]);
		VKLIST_FREE(VKHandle_t, vk_backend.purgeBuffers[i]);
		VKLIST_FREE(VKHandle_t, vk_backend.purgeImages[i]);
	}

	VKMAP_FREE(VkSampler, int, vk_backend.samplerPool);
	VKLIST_FREE(VKShaderProgram_t *, vk_backend.programs);

	R_VK_SavePipelineCache();

	VKZERO(vk_backend);

	vk_backend.initialized = false;
}

/**
 * 终止渲染器
 */
void R_VK_Shutdown()
{
	R_VK_ShutdownBuffer();
	R_VK_ShutdownBackend();

	VK_Destroy();
}

/**
 * 使用staging缓冲区上传缓冲区的GPU内存
 */
void R_VK_UploadBufferData(VKBuffer_t *dstBuffer, int size, const void *data, int dstOffset)
{
	if(!data)
		return;

	VkCommandBuffer commandBuffer = vk_buffer.currentStageWriteCommandBuffer;
	VKBufferMap_t map;

	VKZERO(map);
	R_VK_AllocStageWriteBuffer(size, &map);
	//VK_WriteBuffer(map.buffer, data, map.offset, size, map.ptr);
	memcpy(map.ptr, data, size);

	VK_CopyBufferToBuffer(dstBuffer, map.buffer, dstOffset, map.offset, size, commandBuffer);
}

/**
 * 使用staging缓冲区上传图像的GPU内存
 */
void R_VK_UploadImageData(VKImage_t *image, int totalSize, const byte *totalData, int offsets[], int sizes[])
{
	if(!totalData)
		return;

	VkCommandBuffer commandBuffer = vk_buffer.currentStageWriteCommandBuffer;
	VKBufferMap_t map;

	VKZERO(map);
	R_VK_AllocStageWriteBuffer(totalSize, &map);
    //VK_WriteBuffer(map.buffer, totalData, map.offset, totalSize, map.ptr);
	memcpy(map.ptr, totalData, totalSize);

	VK_WriteImageBuffer(image, map.buffer, map.offset, offsets, sizes, commandBuffer);
}

void R_VK_UploadImageSubData(VKImage_t *image, int totalSize, const byte *totalData, int level, int offsets[], int sizes[])
{
	if(!totalData)
		return;

	VkCommandBuffer commandBuffer = vk_buffer.currentStageWriteCommandBuffer;
	VKBufferMap_t map;

	VKZERO(map);
	R_VK_AllocStageWriteBuffer(totalSize, &map);
    //VK_WriteBuffer(map.buffer, totalData, map.offset, totalSize, map.ptr);
	memcpy(map.ptr, totalData, totalSize);

	VK_CopyBufferToImage(image, map.buffer, map.offset, 1, sizes, offsets, commandBuffer);
}

/**
 * 绑定图像到当前图像单元
 */
void R_VK_Bind(const VKImage_t *image)
{
	assert(vk_backend.currenttmu >= 0 && vk_backend.currenttmu < VKBINDING_MAX_TRXTURE_UNITS);
	if(image)
	{
		if(image->cubeMap)
			vk_backend.tmu[TUT_CUBEMAP][vk_backend.currenttmu] = image;
		else if(image->layers > 1)
			vk_backend.tmu[TUT_2D_ARRAY][vk_backend.currenttmu] = image;
		else
			vk_backend.tmu[TUT_2D][vk_backend.currenttmu] = image;
	}
	else
	{
		vk_backend.tmu[TUT_2D][vk_backend.currenttmu] = NULL;
		vk_backend.tmu[TUT_CUBEMAP][vk_backend.currenttmu] = NULL;
		vk_backend.tmu[TUT_2D_ARRAY][vk_backend.currenttmu] = NULL;
	}
}

void R_VK_BindType(int tut, const VKImage_t *image)
{
	assert(tut >= TUT_2D && tut < TUT_TOTAL);
	assert(vk_backend.currenttmu >= 0 && vk_backend.currenttmu < VKBINDING_MAX_TRXTURE_UNITS);

	vk_backend.tmu[tut][vk_backend.currenttmu] = image;
}

/**
 * 选择当前图像单元
 */
void R_VK_SelectTexture(int unit)
{
	assert(unit >= 0 && unit < VKBINDING_MAX_TRXTURE_UNITS);
	vk_backend.currenttmu = unit;
}

/**
 * 绑定图像到图像单元
 */
void R_VK_BindTexture(int unit, const VKImage_t *image)
{
	assert(unit >= 0 && unit < VKBINDING_MAX_TRXTURE_UNITS);
	vk_backend.currenttmu = unit;
	R_VK_Bind(image);
}

void R_VK_BindTextureType(int unit, int tut, const VKImage_t *image)
{
	assert(unit >= 0 && unit < VKBINDING_MAX_TRXTURE_UNITS);
	assert(tut >= TUT_2D && tut < TUT_TOTAL);
	vk_backend.currenttmu = unit;
	R_VK_BindType(tut, image);
}

/**
 * 从全局uniform拷贝到当前将要渲染的渲染管线的uniform
 */
static void R_VK_CopyAllUniforms()
{
	ASSERT_PROGRAM;

	vk_backend.currentProgram->uboUpdate = false;
	if(vk_backend.currentProgram->copyUniform_vertex)
	{
		vk_backend.currentProgram->copyUniform_vertex();
		if (vk_backend.currentProgram->uboUpdate)
		{
			vk_backend.currentProgram->uboUpdateMask |= UTM_VERTEX;
			vk_backend.currentProgram->uboUpdate = false;
		}
	}

	if(vk_backend.currentProgram->copyUniform_fragment)
	{
		vk_backend.currentProgram->copyUniform_fragment();
		if (vk_backend.currentProgram->uboUpdate)
		{
			vk_backend.currentProgram->uboUpdateMask |= UTM_FRAGMENT;
			vk_backend.currentProgram->uboUpdate = false;
		}
	}

	if(vk_backend.currentProgram->numTextureUnits > 0)
	{
		//memcpy(vk_backend.currentProgram->textures, vk_backend.tmu, vk_backend.currentProgram->numTextureUnits * sizeof(VKImage_t *));
		for (int i = 0; i < vk_backend.currentProgram->numTextureUnits; i++)
		{
			const int textureType = vk_backend.currentProgram->textureTypes[i];
			if (vk_backend.currentProgram->textures[i] != vk_backend.tmu[textureType][i])
			{
				vk_backend.currentProgram->textures[i] = vk_backend.tmu[textureType][i];

				vk_backend.currentProgram->uboUpdateMask |= (UTM_IMAGE << i);
			}
		}
	}

	vk_backend.currentProgram->frameCount = vk_backend.frameCount; 
}

/**
 * 实际销毁当前帧待销毁的图像
 */
static void R_VK_DestroyPurgeImages()
{
	VKLIST_FOREACH(idImage, vk_backend.purgeImages[vk_context.frameIndex])
	{
		VK_FreeImage(vk_backend.purgeImages[vk_context.frameIndex].list[i]);
	}
	vk_backend.purgeImages[vk_context.frameIndex].num = 0;
}

void R_VK_Unbind(VKImage_t *image)
{
	for(int m = TUT_2D; m < TUT_TOTAL; m++)
	{
		for(int i = 0; i < VKBINDING_MAX_TRXTURE_UNITS; i++)
		{
			if(image == vk_backend.tmu[m][i])
				vk_backend.tmu[m][i] = NULL;
		}
	}

	VKLIST_FOREACH(VKShaderProgram_t *, vk_backend.programs)
	{
		for (int m = 0; m < vk_backend.programs.list[i]->numTextureUnits; m++)
		{
			if (vk_backend.programs.list[i]->textures[m] == image)
			{
				vk_backend.programs.list[i]->textures[m] = NULL;
			}
		}
	}
}

/**
 * 将当前帧待销毁的图像追加到图像销毁队列
 */
void R_VK_DestroyImage(VKHandle_t handle)
{
	if (!vk_backend.initialized)
		return;

	if (handle == VKHANDLE_INVALID)
		return;

	VKImage_t *image = vk_context.images.list[handle];
	if(!image)
		return;
	R_VK_Unbind(image);

	// 如果不在主渲染上下文中, 则追加到下一帧的延迟销毁队列. 否则会在R_VK_BeginFrame开始时直接删除被上一帧引用的对象
	const uint32_t frame = vk_context.inRenderFrame ? vk_context.frameIndex : ((vk_context.frameIndex + 1) % NUM_INFLIGHT_FRAME);

	VKLIST_FOREACH(VKHandle_t, vk_backend.purgeImages[frame])
	{
		if(vk_backend.purgeImages[frame].list[i] == handle)
			return;
	}
	VKLIST_ALLOC(VKHandle_t, vk_backend.purgeImages[frame]);
	*_vkc_value = handle;
}

/**
 * 使用当前的渲染管线布局的描述符布局分配新的描述符集
 */
static VkDescriptorSet R_VK_CreateDescriptorSetForUpdate()
{
	VkDescriptorSet *descriptorSet;
	VkDescriptorPool descriptorPool;
	VKHandle_t handle = VK_AllocDescriptorSet(&descriptorSet, &descriptorPool, vk_backend.currentProgram->numUBO, vk_backend.currentProgram->numTextureUnits);

	VK_CreateDescriptorSet(&descriptorSet, &descriptorPool, &vk_backend.currentProgram->pipelineLayout->descriptorSetLayout, 1, vk_backend.currentProgram->numUBO, vk_backend.currentProgram->numTextureUnits);
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_DESCRIPTOR_SET, (uint64_t)*descriptorSet, "descriptorSet::%d::%s_%d_%d", handle, vk_backend.currentProgram->name, vk_backend.currentProgram->numUBO, vk_backend.currentProgram->numTextureUnits);

	return *descriptorSet;
}

/**
 * 写入当前渲染管线的UBO到当前帧的UBO
 */
static void R_VK_WriteUniformBufferObject(const void *data, int size, VKBufferMap_t *map)
{
	VKBufferQueue_t *cache = &vk_buffer.currentUniformBuffer;
	R_VK_AllocBufferQueue(cache, size, map);
	//void *ptr = R_VK_MapBufferQueue(cache, map);
	memcpy(map->ptr, data, size);
	//R_VK_UnmapBufferQueue(cache);
}

void R_VK_AllocVertexBuffer(const void *data, int size, VKBufferMap_t *map)
{
	VKBufferQueue_t *cache = &vk_buffer.currentVertexBuffer;
	R_VK_AllocBufferQueue(cache, size, map);
	//void *ptr = R_VK_MapBufferQueue(cache, map);
	memcpy(map->ptr, data, size);
	//R_VK_UnmapBufferQueue(cache);
}

void R_VK_AllocIndexBuffer(const void *data, int size, VKBufferMap_t *map)
{
	VKBufferQueue_t *cache = &vk_buffer.currentIndexBuffer;
	R_VK_AllocBufferQueue(cache, size, map);
	//void *ptr = R_VK_MapBufferQueue(cache, map);
	memcpy(map->ptr, data, size);
	//R_VK_UnmapBufferQueue(cache);
}

/**
 * 实际销毁当前帧待销毁的缓冲区
 */
static void R_VK_DestroyPurgeBuffers()
{
	VKLIST_FOREACH(VKHandle_t, vk_backend.purgeBuffers[vk_context.frameIndex])
	{
		VK_FreeBuffer(vk_backend.purgeBuffers[vk_context.frameIndex].list[i]);
	}
	vk_backend.purgeBuffers[vk_context.frameIndex].num = 0;
}

/**
 * 将当前帧待销毁的缓冲区追加到缓冲区销毁队列
 */
void R_VK_DestroyBuffer(VKHandle_t handle)
{
	if (!vk_backend.initialized)
		return;

	if (handle == VKHANDLE_INVALID)
		return;

	// 如果不在主渲染上下文中, 则追加到下一帧的延迟销毁队列. 否则会在R_VK_BeginFrame开始时直接删除被上一帧引用的对象
	const uint32_t frame = vk_context.inRenderFrame ? vk_context.frameIndex : ((vk_context.frameIndex + 1) % NUM_INFLIGHT_FRAME);

	VKLIST_FOREACH(VKHandle_t, vk_backend.purgeBuffers[frame])
	{
		if(vk_backend.purgeBuffers[frame].list[i] == handle)
			return;
	}
	VKLIST_ALLOC(VKHandle_t, vk_backend.purgeBuffers[frame]);
	*_vkc_value = handle;
}

void R_VK_ResetGlobalState()
{
	memset(vk_backend.tmu, 0, sizeof(vk_backend.tmu));
	vk_backend.frameCount++;
	vk_backend.currentDescriptorSet = VK_NULL_HANDLE;
	for(int i = 0; i < vk_backend.programs.num; i++)
	{
		vk_backend.programs.list[i]->descriptorSet = VK_NULL_HANDLE;
	}
}

/**
 * 开始渲染新帧
 */
void R_VK_BeginFrame()
{
    // 上下文开始下一帧
    VK_BeginFrame();
	// 重置该帧要用到的临时缓冲区
	R_VK_VertexCacheBeginFrame();
	// 销毁上一次该帧的要释放的缓冲区
	R_VK_DestroyPurgeBuffers();
	// 销毁上一次该帧的要释放的图像
	R_VK_DestroyPurgeImages();
	// 获取上一次该帧的查询的结果
	R_VK_GetQueryResult();
	// 重置上一次该帧的进行的查询(必须在渲染通道未开始前执行)
	R_VK_ResetQuery();

    // 发送开始主渲染通道的命令
    VK_BeginRenderPass();
	// 后端记录帧数+1
	R_VK_ResetGlobalState();
	// 重置渲染管线绑定
	//R_VK_UseProgram(NULL);

	// 执行管线可变的状态
	R_VK_ExecuteVariableCmd();
}

/**
 * 结束渲染当前帧
 */
void R_VK_EndFrame()
{
    // 发送结束主渲染通道命令
    VK_EndRenderPass();

	// 上传图像缓冲区
	R_VK_HandleLoadImageList();

	// 提交该帧要用到的临时缓冲区
	R_VK_VertexCacheEndFrame();

	// 上下文结束本帧
	VK_EndFrame();
}

/**
 * 更新当前渲染管线的uniform数据, 准备发送绘制命令
 */
void R_VK_UpdateUniformBufferObject()
{
	R_VK_SyncPipelineState();

	R_VK_CopyAllUniforms();

	if (vk_backend.currentProgram->uboUpdateMask == 0 && VK_NULL_HANDLE != vk_backend.currentProgram->descriptorSet)
	{
		if (vk_backend.currentDescriptorSet != vk_backend.currentProgram->descriptorSet)
		{
			vkCmdBindDescriptorSets(vk_context.currentFrameCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_backend.currentProgram->pipelineLayout->pipelineLayout, 0, 1, &vk_backend.currentProgram->descriptorSet, 0, NULL);
			vk_backend.currentDescriptorSet = vk_backend.currentProgram->descriptorSet;
		}
		return;
	}

	vk_backend.currentProgram->descriptorSet = VK_NULL_HANDLE;

    VkWriteDescriptorSet writeDescriptorSets[VKBINDING_NUM];
	VKZERO(writeDescriptorSets);
	int numWriteDescriptorSets = 0;
	VkDescriptorBufferInfo descriptorBufferInfo0;
	VkDescriptorBufferInfo descriptorBufferInfo1;

	VkDescriptorSet descriptorSet = R_VK_CreateDescriptorSetForUpdate();

	if(vk_backend.currentProgram->ubo_vertex)
	{
		if(vk_backend.currentProgram->uboUpdateMask & UTM_VERTEX)
			R_VK_WriteUniformBufferObject(vk_backend.currentProgram->ubo_vertex, vk_backend.currentProgram->uboSize_vertex, &vk_backend.currentProgram->uniformBuffer_vertex);

		VKZERO(descriptorBufferInfo0);
		descriptorBufferInfo0.buffer = vk_backend.currentProgram->uniformBuffer_vertex.buffer->buffer;
		descriptorBufferInfo0.offset = vk_backend.currentProgram->uniformBuffer_vertex.offset;
		descriptorBufferInfo0.range = vk_backend.currentProgram->uniformBuffer_vertex.size;

		writeDescriptorSets[numWriteDescriptorSets].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[numWriteDescriptorSets].dstSet = descriptorSet;
		writeDescriptorSets[numWriteDescriptorSets].dstBinding = 0;
		writeDescriptorSets[numWriteDescriptorSets].dstArrayElement = 0;
		writeDescriptorSets[numWriteDescriptorSets].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets[numWriteDescriptorSets].descriptorCount = 1;
		writeDescriptorSets[numWriteDescriptorSets].pBufferInfo = &descriptorBufferInfo0;
		writeDescriptorSets[numWriteDescriptorSets].pImageInfo = NULL;
		writeDescriptorSets[numWriteDescriptorSets].pTexelBufferView = NULL;

		numWriteDescriptorSets++;
	}

	if(vk_backend.currentProgram->ubo_fragment)
	{
		if(vk_backend.currentProgram->uboUpdateMask & UTM_FRAGMENT)
			R_VK_WriteUniformBufferObject(vk_backend.currentProgram->ubo_fragment, vk_backend.currentProgram->uboSize_fragment, &vk_backend.currentProgram->uniformBuffer_fragment);

		VKZERO(descriptorBufferInfo1);
		descriptorBufferInfo1.buffer = vk_backend.currentProgram->uniformBuffer_fragment.buffer->buffer;
		descriptorBufferInfo1.offset = vk_backend.currentProgram->uniformBuffer_fragment.offset;
		descriptorBufferInfo1.range = vk_backend.currentProgram->uniformBuffer_fragment.size;

		writeDescriptorSets[numWriteDescriptorSets].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[numWriteDescriptorSets].dstSet = descriptorSet;
		writeDescriptorSets[numWriteDescriptorSets].dstBinding = 1;
		writeDescriptorSets[numWriteDescriptorSets].dstArrayElement = 0;
		writeDescriptorSets[numWriteDescriptorSets].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets[numWriteDescriptorSets].descriptorCount = 1;
		writeDescriptorSets[numWriteDescriptorSets].pBufferInfo = &descriptorBufferInfo1;
		writeDescriptorSets[numWriteDescriptorSets].pImageInfo = NULL;
		writeDescriptorSets[numWriteDescriptorSets].pTexelBufferView = NULL;
		numWriteDescriptorSets++;
	}

	VkDescriptorImageInfo descriptorImageInfos[VKBINDING_MAX_TRXTURE_UNITS];
	if (vk_backend.currentProgram->numTextureUnits > 0)
	{
		for (int i = 0; i < vk_backend.currentProgram->numTextureUnits; i++)
		{
			//if (vk_backend.currentProgram->uboUpdateMask & (UTM_IMAGE << i))
			{
				VKZERO(descriptorImageInfos[i]);
				const VKImage_t *image = vk_backend.currentProgram->textures[i];
				if (!image) // always binding one
				 	image = vk_backend.currentProgram->textureTypes[i] == TUT_CUBEMAP ? globalImages->defaultCubeMapImage->vk_image : globalImages->defaultImage->vk_image;

				descriptorImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				descriptorImageInfos[i].imageView = image->imageView;
				descriptorImageInfos[i].sampler = image->sampler;

				writeDescriptorSets[numWriteDescriptorSets].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDescriptorSets[numWriteDescriptorSets].dstSet = descriptorSet;
				writeDescriptorSets[numWriteDescriptorSets].dstBinding = VKBINDING_TRXTURES_SLOT(i);
				writeDescriptorSets[numWriteDescriptorSets].dstArrayElement = 0;
				writeDescriptorSets[numWriteDescriptorSets].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				writeDescriptorSets[numWriteDescriptorSets].descriptorCount = 1;
				writeDescriptorSets[numWriteDescriptorSets].pBufferInfo = NULL;
				writeDescriptorSets[numWriteDescriptorSets].pImageInfo = &descriptorImageInfos[i];
				writeDescriptorSets[numWriteDescriptorSets].pTexelBufferView = NULL;
				numWriteDescriptorSets++;
			}
		}
	}

	vk_backend.currentProgram->uboUpdate = false;
	vk_backend.currentProgram->uboUpdateMask = 0;

    vkUpdateDescriptorSets(vk_context.device, numWriteDescriptorSets, writeDescriptorSets, 0, NULL);

	vkCmdBindDescriptorSets(vk_context.currentFrameCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_backend.currentProgram->pipelineLayout->pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

	vk_backend.currentProgram->descriptorSet = descriptorSet;
	vk_backend.currentDescriptorSet = descriptorSet;
}

void R_VK_CmdDraw(int vertexCount, int firstVertex)
{
	R_VK_UpdateUniformBufferObject();
	vkCmdDraw(vk_context.currentFrameCommandBuffer, vertexCount, 1, firstVertex, 0);
}

void R_VK_CmdDrawInstance(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
{
	R_VK_UpdateUniformBufferObject();
	vkCmdDraw(vk_context.currentFrameCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void R_VK_CmdDrawIndexed(int indexCount, int firstIndex, int vertexOffset)
{
	R_VK_UpdateUniformBufferObject();
	vkCmdDrawIndexed(vk_context.currentFrameCommandBuffer, indexCount, 1, firstIndex, vertexOffset, 0);
}

void R_VK_CmdDrawIndexedInstance(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
{
	R_VK_UpdateUniformBufferObject();
	vkCmdDrawIndexed(vk_context.currentFrameCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void R_VK_CmdBindIndexBuffer(VkBuffer buffer, int offset)
{
	vkCmdBindIndexBuffer(vk_context.currentFrameCommandBuffer, buffer, offset, VK_INDEX_TYPE_UINT32);
}

void R_VK_CmdBindVertexBuffer(VkBuffer pBuffer, VkDeviceSize offset)
{
	vkCmdBindVertexBuffers(vk_context.currentFrameCommandBuffer, 0, 1, &pBuffer, &offset);
}

void R_VK_CmdBindVertexBuffers(int bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
	vkCmdBindVertexBuffers(vk_context.currentFrameCommandBuffer, 0, bindingCount, pBuffers, pOffsets);
}

#define VKSTATE1_SET_BOOL(b, x) if(b) { vk_backend.state.st1 |= (x); } else { vk_backend.state.st1 &= ~(globalState_t)(x); }
#define VKSTATE1_UNSET_BOOL(b, x) if(b) { vk_backend.state.st1 &= ~(globalState_t)(x); } else { vk_backend.state.st1 |= (x); }
#define VKSTATE1_SET(x) if((x) > 0) { vk_backend.state.st1 |= HIGHI_BIT(x); }

#define VKSTATE2_SET_BOOL(b, x) if(b) { vk_backend.state.st2 |= (x); } else { vk_backend.state.st2 &= ~(globalState_t)(x); }
#define VKSTATE2_UNSET_BOOL(b, x) if(b) { vk_backend.state.st2 &= ~(globalState_t)(x); } else { vk_backend.state.st2 |= (x); }
#define VKSTATE2_SET(x) if((x) > 0) { vk_backend.state.st2 |= HIGHI_BIT(x); }

#define VKSTATE1_IS_TRUE(x) (vk_backend.state.st1 & (x))
#define VKSTATE2_IS_TRUE(x) (vk_backend.state.st2 & (x))

/**
 * 渲染管线固定状态: 变更时创建新的渲染管线, 主要是针对状态开关类型的状态
 * 深度测试
 * 模版测试
 * 混合
 * 裁剪测试
 * 写入深度
 * 写入颜色
 * 多边形模式
 * 图元类型
 * 多边形环绕模式
 * 裁剪面
 * 深度比较方法
 * 源/目标混合方法
*/

/**
 * 渲染管线可变状态: 绑定管线时动态重新设置, 必须先开启命令缓冲区, 主要是针对具体数值类型的状态
 * 视口大小
 * 裁剪框大小
 * 线宽度
 * 模版正反面参考值/写入掩码
 */

void R_VK_EnableDepthTest(bool enabled)
{
	VKSTATE1_SET_BOOL(enabled, ST_DEPTH_TEST_BIT);
}

void R_VK_EnableStencilTest(bool enabled)
{
	VKSTATE1_SET_BOOL(enabled, ST_STENCIL_TEST_BIT);
}

void R_VK_EnableBlend(bool enabled)
{
	VKSTATE1_SET_BOOL(enabled, ST_BLEND_BIT);
}

void R_VK_EnableScissorTest(bool enabled)
{
	VKSTATE1_SET_BOOL(enabled, ST_SCISSOR_TEST_BIT);
	if (vk_context.inCommandRecording)
	{
		if(enabled)
			VK_Scissor(vk_backend.dynamicState.scissor[0], vk_backend.dynamicState.scissor[1], vk_backend.dynamicState.scissor[2], vk_backend.dynamicState.scissor[3]);
		else
			VK_Scissor(0, 0, vk_context.swapchain.width, vk_context.swapchain.height);
	}
}

bool R_VK_IsDepthTestEnabled()
{
	return VKSTATE1_IS_TRUE(ST_DEPTH_TEST_BIT);
}

bool R_VK_IsStencilTestEnabled()
{
	return VKSTATE1_IS_TRUE(ST_STENCIL_TEST_BIT);
}

bool R_VK_IsBlendEnabled()
{
	return VKSTATE1_IS_TRUE(ST_BLEND_BIT);
}

bool R_VK_IsScissorTestEnabled()
{
	return VKSTATE1_IS_TRUE(ST_SCISSOR_TEST_BIT);
}

bool R_VK_IsCullFaceEnabled()
{
	return VKSTATE1_IS_TRUE(ST_CULL_FACE_BIT);
}

bool R_VK_IsPolygonOffsetEnabled()
{
	return VKSTATE1_IS_TRUE(ST_POLYGON_OFFSET_BIT);
}

void R_VK_DepthMask(bool write)
{
	VKSTATE1_UNSET_BOOL(write, ST_DEPTH_MASK_BIT);
}

void R_VK_ColorMask(bool r, bool g, bool b, bool a)
{
	VKSTATE1_UNSET_BOOL(r, ST_COLORMASK_R_BIT);
	VKSTATE1_UNSET_BOOL(g, ST_COLORMASK_G_BIT);
	VKSTATE1_UNSET_BOOL(b, ST_COLORMASK_B_BIT);
	VKSTATE1_UNSET_BOOL(a, ST_COLORMASK_A_BIT);
}

void R_VK_PolygonMode(int mode)
{
	VKSTATE1_CLEAR(ST_POLYGON_BITS);
	VKSTATE1_SET(mode);
}

void R_VK_Begin(int mode)
{
	VKSTATE1_CLEAR(ST_PRIMITIVE_BITS);
	VKSTATE1_SET(mode);
}

void R_VK_EnableCullFace(bool enabled)
{
    R_VK_CullFace(enabled ? vk_backend.dynamicState.cullFace : ST_CULL_NONE);
}

void R_VK_FrontFace(int mode)
{
	VKSTATE1_SET_BOOL(mode, ST_FRONT_CW_BIT);
}

void R_VK_CullFace(int face)
{
    vk_backend.dynamicState.cullFace = face;
	VKSTATE1_CLEAR(ST_CULL_BITS);
	if(face == ST_CULL_FRONT_AND_BACK)
	    vk_backend.state.st1 |= (ST_CULL_FRONT_BIT | ST_CULL_BACK_BIT);
	else
	    VKSTATE1_SET(face);
}

void R_VK_EnablePolygonOffset(bool enabled)
{
	VKSTATE1_SET_BOOL(enabled, ST_POLYGON_OFFSET_BIT);
}

void R_VK_DepthFunc(int func)
{
	VKSTATE1_CLEAR(ST_DEPTHFUNC_BITS);
	VKSTATE1_SET(func);
}

void R_VK_BlendFunc(int src, int dst)
{
	VKSTATE1_CLEAR(ST_BLEND_BITS);
	VKSTATE1_SET(src);
	VKSTATE1_SET(dst);
}

void R_VK_StencilOp(int sfail, int dpfail, int dppass)
{
	VKSTATE2_CLEAR(ST_STENCILOP_BITS);
	if (sfail)
	{
		VKSTATE2_SET(sfail);
		sfail += ST_STENCILOP_BACK_STRIDE;
		VKSTATE2_SET(sfail);
	}
	if (dppass)
	{
		dppass += ST_STENCILOP_PASS_STRIDE;
		VKSTATE2_SET(dppass);
		dppass += ST_STENCILOP_BACK_STRIDE;
		VKSTATE2_SET(dppass);
	}
	if (dpfail)
	{
		dpfail += ST_STENCILOP_DEPTHFAIL_STRIDE;
		VKSTATE2_SET(dpfail);
		dpfail += ST_STENCILOP_BACK_STRIDE;
		VKSTATE2_SET(dpfail);
	}
}

void R_VK_StencilOpSeparate(int face, int sfail, int dpfail, int dppass)
{
	if (face == ST_CULL_FRONT)
	{
		VKSTATE2_CLEAR(ST_STENCIL_FRONT_OP_BITS);
		if (sfail)
		{
			VKSTATE2_SET(sfail);
		}
		if (dppass)
		{
			dppass += ST_STENCILOP_PASS_STRIDE;
			VKSTATE2_SET(dppass);
		}
		if (dpfail)
		{
			dpfail += ST_STENCILOP_DEPTHFAIL_STRIDE;
			VKSTATE2_SET(dpfail);
		}
	}
	else if (face == ST_CULL_BACK)
	{
		VKSTATE2_CLEAR(ST_STENCIL_BACK_OP_BITS);
		if (sfail)
		{
			sfail += ST_STENCILOP_BACK_STRIDE;
			VKSTATE2_SET(sfail);
		}
		if (dppass)
		{
			dppass += ST_STENCILOP_PASS_STRIDE;
			dppass += ST_STENCILOP_BACK_STRIDE;
			VKSTATE2_SET(dppass);
		}
		if (dpfail)
		{
			dpfail += ST_STENCILOP_DEPTHFAIL_STRIDE;
			dpfail += ST_STENCILOP_BACK_STRIDE;
			VKSTATE2_SET(dpfail);
		}
	}
	else
	    R_VK_StencilOp(sfail, dpfail, dppass);
}

void R_VK_StencilMask(unsigned int mask)
{
	vk_backend.dynamicState.stencilWriteMask[0] = vk_backend.dynamicState.stencilWriteMask[1] = mask;

	if (vk_context.inCommandRecording)
		VK_StencilMask(mask);
}

void R_VK_StencilMaskSeparate(int face, unsigned int mask)
{
	if (face == ST_CULL_FRONT)
	{
		vk_backend.dynamicState.stencilWriteMask[0] = mask;

		if (vk_context.inCommandRecording)
			VK_StencilMaskSeparate(VK_STENCIL_FACE_FRONT_BIT, mask);
	}
	else if (face == ST_CULL_BACK)
	{
		vk_backend.dynamicState.stencilWriteMask[1] = mask;

		if (vk_context.inCommandRecording)
			VK_StencilMaskSeparate(VK_STENCIL_FACE_BACK_BIT, mask);
	}
	else
		R_VK_StencilMask(mask);
}

void R_VK_DrawArrays(int mode, int first, int count)
{
	globalState_t lastSt1 = vk_backend.state.st1;
	R_VK_Begin(mode);
	R_VK_CmdDraw(count, first);
	vk_backend.state.st1 = lastSt1;
}

void R_VK_Clear(bool color, bool depth, bool stencil)
{
	if (!vk_context.inCommandRecording)
		return;

    int mask = 0;
    if(color)
        mask |= VK_IMAGE_ASPECT_COLOR_BIT;
    if(depth)
        mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if(stencil)
        mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    if(mask == 0)
        return;

    VK_Clear(mask, vk_backend.dynamicState.clearColor[0], vk_backend.dynamicState.clearColor[1], vk_backend.dynamicState.clearColor[2], vk_backend.dynamicState.clearColor[3], vk_backend.dynamicState.clearDepth, vk_backend.dynamicState.clearStencil);
}

void R_VK_StencilFunc(int func, int ref, unsigned int mask)
{
	VKSTATE2_CLEAR(ST_STENCILFUNC_BITS);
	if(func)
	{
		VKSTATE2_SET(func);
		func += ST_STENCILFUNC_BACK_STRIDE;
		VKSTATE2_SET(func);
	}
	vk_backend.dynamicState.stencilReference[0] = vk_backend.dynamicState.stencilReference[1] = ref;
	vk_backend.dynamicState.stencilMask[0] = vk_backend.dynamicState.stencilMask[1] = mask;

	if (vk_context.inCommandRecording)
		VK_StencilFunc(ref, mask);
}

void R_VK_StencilFuncSeparate(int face, int func, int ref, unsigned int mask)
{
	if (face == ST_CULL_FRONT)
	{
		VKSTATE2_CLEAR(ST_STENCILFUNC_FRONT_BITS);
		if (func)
		{
			VKSTATE2_SET(func);
		}
		vk_backend.dynamicState.stencilReference[0] = ref;
		vk_backend.dynamicState.stencilMask[0] = mask;
		
		if (vk_context.inCommandRecording)
			VK_StencilFuncSeparate(VK_STENCIL_FACE_FRONT_BIT, ref, mask);
	}
	else if (face == ST_CULL_BACK)
	{
		VKSTATE2_CLEAR(ST_STENCILFUNC_BACK_BITS);
		if (func)
		{
			func += ST_STENCILFUNC_BACK_STRIDE;
			VKSTATE2_SET(func);
		}
		vk_backend.dynamicState.stencilReference[1] = ref;
		vk_backend.dynamicState.stencilMask[1] = mask;

		if (vk_context.inCommandRecording)
			VK_StencilFuncSeparate(VK_STENCIL_FACE_BACK_BIT, ref, mask);
	}
	else
	    R_VK_StencilFunc(func, ref, mask);
}

void R_VK_Viewport(int x, int y, int w, int h)
{
	vk_backend.dynamicState.viewport[0] = x;
	vk_backend.dynamicState.viewport[1] = y;
	vk_backend.dynamicState.viewport[2] = w;
	vk_backend.dynamicState.viewport[3] = h;

	if (vk_context.inCommandRecording)
		VK_Viewport(x, y, w, h, vk_backend.dynamicState.depthRanges[0], vk_backend.dynamicState.depthRanges[1]);
}

void R_VK_DepthRange(float minZ, float maxZ)
{
	vk_backend.dynamicState.depthRanges[0] = minZ;
	vk_backend.dynamicState.depthRanges[1] = maxZ;

	if (vk_context.inCommandRecording)
		VK_Viewport(vk_backend.dynamicState.viewport[0], vk_backend.dynamicState.viewport[1], vk_backend.dynamicState.viewport[2], vk_backend.dynamicState.viewport[3], minZ, maxZ);
}

void R_VK_Scissor(int x, int y, int w, int h)
{
	vk_backend.dynamicState.scissor[0] = x;
	vk_backend.dynamicState.scissor[1] = y;
	vk_backend.dynamicState.scissor[2] = w;
	vk_backend.dynamicState.scissor[3] = h;

	if (vk_context.inCommandRecording)
	{
		if(VKSTATE1_BIT(vk_backend.state, ST_SCISSOR_TEST_BIT))
			VK_Scissor(x, y, w, h);
		else
			VK_Scissor(0, 0, vk_context.swapchain.width, vk_context.swapchain.height);
	}
}

void R_VK_LineWidth(float width)
{
	vk_backend.dynamicState.lineWidth = width;

	if (vk_context.inCommandRecording)
		VK_LineWidth(width);
}

void R_VK_PolygonOffset(float factor, float units)
{
    vk_backend.dynamicState.polygonFactor = factor;
    vk_backend.dynamicState.polygonUnits = units;

	if (vk_context.inCommandRecording)
        VK_PolygonOffset(factor, units);
}

void R_VK_ClearColor(float r, float g, float b, float a)
{
	vk_backend.dynamicState.clearColor[0] = r;
	vk_backend.dynamicState.clearColor[1] = g;
	vk_backend.dynamicState.clearColor[2] = b;
	vk_backend.dynamicState.clearColor[3] = a;
}

void R_VK_ClearDepth(float depth)
{
	vk_backend.dynamicState.clearDepth = depth;
}

void R_VK_ClearStencil(int stencil)
{
	vk_backend.dynamicState.clearStencil = stencil;
}

void R_VK_Color4f(float r, float g, float b, float a)
{
	R_VK_Uniform4f(VKGLOBAL_PARM_OFFSET(glColor), r, g, b, a);
}

void R_VK_Color4fv(float color[4])
{
	R_VK_Uniform4fv(VKGLOBAL_PARM_OFFSET(glColor), color);
}

static const float ColorModulate = 1.0f;

void R_VK_UseVertexColor(bool enable)
{
	if(enable)
	{
		R_VK_Uniform1f(VKGLOBAL_PARM_OFFSET(colorModulate), ColorModulate);
		R_VK_Uniform1f(VKGLOBAL_PARM_OFFSET(colorAdd), 0.0f);
	}
	else
	{
		R_VK_Uniform1f(VKGLOBAL_PARM_OFFSET(colorModulate), 0.0f);
		R_VK_Uniform1f(VKGLOBAL_PARM_OFFSET(colorAdd), 1.0f);
	}
}

void R_VK_RecreateImage(VKHandle_t texnum, VKImage_t **image)
{
	VKImage_t *newImage;
	VKHandle_t newHandle = VK_AllocImage(&newImage);
	VK_SwapImageHandle(texnum, newHandle);
	R_VK_DestroyImage(newHandle);
	*image = newImage;
}

void R_VK_SetObjectName(int objType, uint64_t handle, const char *name, ...)
{
	if (!r_vkSetDebugInfo.GetBool() || !glConfig.debugOutput)
		return;

	if ((uint64_t)VK_NULL_HANDLE == handle)
		return;

	if (name)
	{
		char buffer[256];
		va_list ap;
		va_start(ap, name);
		vsprintf(buffer, name, ap);
		va_end(ap);
		VK_SetObjectName(objType, handle, buffer);
	}
	else
		VK_SetObjectName(objType, handle, "");
}

void R_VK_CopyColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight)
{
#if 0
	if(width == dstWidth && height == dstHeight)
		VK_CopyColorAttachmentImage(image, x, y, width, height, dstX, dstY);
	else
#endif
	//VK_BlitColorAttachmentImage(image, x, y + height, width, y, dstX, dstY, dstWidth, dstHeight, false);
	VK_BlitColorAttachmentImage(image, x, y, width, height, dstX, dstY, dstWidth, dstHeight, false);
}

void R_VK_CopyDepthStencilAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight, int mask)
{
#if 0
	if(width == dstWidth && height == dstHeight)
		VK_CopyDepthStencilAttachmentImage(image, x, y, width, height, dstX, dstY, mask);
	else
		VK_BlitDepthStencilAttachmentImage(image, x, y, width, height, dstX, dstY, dstWidth, dstHeight, mask, false);
#endif
	VK_CopyDepthStencilAttachmentImage(image, x, y, width, height, dstX, dstY, mask);
}
