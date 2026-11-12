#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#if IDVK_CONTAINER_SIZE_CONFIG
idCVar r_vkUsePipelineCache("r_vkUsePipelineCache", "0", CVAR_BOOL | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: using pipeline cache");
idCVar r_vkSetDebugInfo("r_vkSetDebugInfo", "0", CVAR_INTEGER | CVAR_RENDERER, "[Vulkan]: setup debug info");
idCVar r_vkEnableValidationLayer("r_vkEnableValidationLayer", "0", CVAR_BOOL | CVAR_RENDERER, "[Vulkan]: setup debug info");

// vk_context
idCVar r_vkContextPipelineSize("r_vkContextPipelineSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global pipeline list initialize size");
idCVar r_vkContextPipelineGranularity("r_vkContextPipelineGranularity", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global pipeline list allocation granularity");
idCVar r_vkContextPipelineLayoutSize("r_vkContextPipelineLayoutSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global pipeline layout list initialize size");
idCVar r_vkContextPipelineLayoutGranularity("r_vkContextPipelineLayoutGranularity", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global pipeline layout list allocation granularity");
idCVar r_vkContextShaderSize("r_vkContextShaderSize", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global shader list initialize size");
idCVar r_vkContextShaderGranularity("r_vkContextShaderGranularity", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global shader list allocation granularity");
idCVar r_vkContextSamplerSize("r_vkContextSamplerSize", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global sampler list initialize size");
idCVar r_vkContextSamplerGranularity("r_vkContextSamplerGranularity", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global sampler list allocation granularity");
idCVar r_vkContextImageViewSize("r_vkContextImageViewSize", "6", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global image view list initialize size");
idCVar r_vkContextImageViewGranularity("r_vkContexImageViewGranularity", "6", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global image view list allocation granularity");
idCVar r_vkContextImageSize("r_vkContextImageSize", "2048", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global image list initialize size");
idCVar r_vkContextImageGranularity("r_vkContextImageGranularity", "2048", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global image list allocation granularity");
idCVar r_vkContextBufferSize("r_vkContextBufferSize", "2048", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global buffer list initialize size");
idCVar r_vkContextBufferGranularity("r_vkContextBufferGranularity", "2048", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: global buffer list allocation granularity");

// vk_backend
idCVar r_vkBackendSamplerSize("r_vkBackendSamplerSize", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: sampler list initialize size");
idCVar r_vkBackendSamplerGranularity("r_vkBackendSamplerGranularity", "128", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: sampler list allocation granularity");
idCVar r_vkBackendFreeBufferSize("r_vkBackendFreeBuffersSize", "1024", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: defer free buffers list initialize size");
idCVar r_vkBackendFreeBufferGranularity("r_vkBackendFreeBuffersGranularity", "1024", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: defer free buffers list allocation granularity");
idCVar r_vkBackendPurgeImageSize("r_vkBackendPurgeImagesSize", "512", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: defer purge images list initialize size");
idCVar r_vkBackendPurgeImageGranularity("r_vkBackendPurgeImagesGranularity", "512", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: defer purge images list allocation granularity");
idCVar r_vkBackendPipelineLayoutSize("r_vkBackendPipelineLayoutSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: pipeline layout list initialize size");
idCVar r_vkBackendPipelineLayoutGranularity("r_vkBackendPipelineLayoutGranularity", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: pipeline layout list allocation granularity");

// vk_query
idCVar r_vkQueryFrameCount("r_vkQueryFrameCount", "256", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: query count in frame");

// vk_buffer
idCVar r_vkStagingWriteBufferBlockSize("r_vkStagingWriteBufferBlockSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: staging write buffer block size(Mb)");
idCVar r_vkStagingReadBufferBlockSize("r_vkStagingReadBufferBlockSize", "16", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: staging read buffer block size(Mb)");
idCVar r_vkUniformBufferBlockSize("r_vkUniformBufferBlockSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: uniform buffer block size(Mb)");
idCVar r_vkVertexBufferBlockSize("r_vkVertexBufferBlockSize", "64", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: vertex buffer block size(Mb)");
idCVar r_vkIndexBufferBlockSize("r_vkIndexBufferBlockSize", "16", CVAR_INTEGER | CVAR_RENDERER | CVAR_ARCHIVE, "[Vulkan]: index buffer block size(Mb)");
#endif

void R_VK_CheckPortableExtensions(void)
{
	static char renderer_string[64];
	static char vendor_string[64];
	static char version_string[64];
	static idStr extensions;

	idStr::snPrintf(renderer_string, sizeof(renderer_string), "%s", vk_context.physicalDeviceProperties.deviceName);
	idStr::snPrintf(vendor_string, sizeof(vendor_string), "0x%X", vk_context.physicalDeviceProperties.vendorID);
	idStr::snPrintf(version_string, sizeof(version_string), "%d.%d.%d", VK_API_VERSION_MAJOR(vk_context.physicalDeviceProperties.apiVersion), VK_API_VERSION_MINOR(vk_context.physicalDeviceProperties.apiVersion), VK_API_VERSION_PATCH(vk_context.physicalDeviceProperties.apiVersion));
	extensions.Clear();
	for (int i = 0; i < vk_context.numExtensions; i++)
	{
		extensions.Append(vk_context.extensions[i]);
		if (i < vk_context.numExtensions - 1)
			extensions.Append(" ");
	}

	glConfig.renderer_string = renderer_string;
	glConfig.vendor_string = vendor_string;
	glConfig.version_string = version_string;
	glConfig.extensions_string = extensions.c_str();
	glConfig.shading_language_version_string = "GLSL 450";
	glConfig.glVersion = vk_context.physicalDeviceProperties.apiVersion;

	glConfig.multitextureAvailable = true;
	glConfig.maxTextureUnits = vk_context.physicalDeviceProperties.limits.maxPerStageDescriptorSampledImages;
	if (glConfig.maxTextureUnits > MAX_MULTITEXTURE_UNITS) {
		glConfig.maxTextureUnits = MAX_MULTITEXTURE_UNITS;
	}
	if (glConfig.maxTextureUnits < 2) {
		glConfig.multitextureAvailable = false;	// shouldn't ever happen
	}
	glConfig.maxTextureCoords = vk_context.physicalDeviceProperties.limits.maxImageDimension2D;
	glConfig.maxTextureImageUnits = vk_context.physicalDeviceProperties.limits.maxPerStageDescriptorSampledImages;

	glConfig.textureEnvCombineAvailable = false;
	glConfig.cubeMapAvailable = true;
	glConfig.envDot3Available = false;
	glConfig.textureEnvAddAvailable = false;
	glConfig.textureNonPowerOfTwoAvailable = true;

	glConfig.textureCompressionAvailable = vk_context.physicalDeviceFeatures.textureCompressionBC;
	glConfig.maxTextureAnisotropy = vk_context.physicalDeviceProperties.limits.maxSamplerAnisotropy;
	glConfig.anisotropicAvailable = vk_context.physicalDeviceProperties.limits.maxSamplerAnisotropy > 1.0f;

	glConfig.anisotropicAvailable = false; // VK:TODO
	glConfig.textureLODBiasAvailable = true;

	glConfig.sharedTexturePaletteAvailable = false;
	glConfig.texture3DAvailable = true;

	// EXT_stencil_wrap
	tr.stencilIncr = GL_INCR_WRAP;
	tr.stencilDecr = GL_DECR_WRAP;

	glConfig.ARBVertexBufferObjectAvailable = true;
	glConfig.ARBVertexProgramAvailable = false;
	glConfig.ARBFragmentProgramAvailable = false;

	glConfig.depthBoundsTestAvailable = false; // vk_context.physicalDeviceFeatures.depthBounds;
	//qglDepthBoundsEXT = NULL;

	glConfig.GLSLAvailable = true;
	glConfig.framebufferObjectAvailable = true;
	glConfig.maxColorAttachments = vk_context.physicalDeviceProperties.limits.maxColorAttachments;
	glConfig.maxRenderbufferSize = idMath::Min(vk_context.physicalDeviceProperties.limits.maxFramebufferWidth, vk_context.physicalDeviceProperties.limits.maxFramebufferHeight);

	glConfig.depthTextureAvailable = true;
	glConfig.depthTextureCubeMapAvailable = true;
	glConfig.depth24Available = true;
	glConfig.gl_FragDepthAvailable = true;

	glConfig.debugOutput = VK_IsExtensionSupported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	glConfig.syncAvailable = true;

	glConfig.isVulkan = true;
}
