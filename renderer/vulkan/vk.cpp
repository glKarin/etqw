#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#include "vk_enum.h"

#ifdef __ANDROID__
#else
#include <windows.h>
#endif

#define VKCONTEXT_ALLOCATOR vk_context.allocator

#define IDVK_DESCRIPTOR_POOL_SIZE_UBO (IDVK_MAX_DESCRIPTOR_POOL_SETS * 2) // vertex shader + fragment shader
#define IDVK_DESCRIPTOR_POOL_SIZE_SAMPLER (IDVK_MAX_DESCRIPTOR_POOL_SETS * 8) // max texture units in fragment shader

#if IDVK_CONTAINER_SIZE_CONFIG

#define IDVK_PILELINE_INIT_SIZE ( r_vkContextPipelineSize.GetInteger() )
#define IDVK_PIPELINE_GRANULARITY ( r_vkContextPipelineGranularity.GetInteger() )
#define IDVK_PIPELINE_LAYOUT_INIT_SIZE ( r_vkContextPipelineLayoutSize.GetInteger() )
#define IDVK_PIPELINE_LAYOUT_GRANULARITY ( r_vkContextPipelineLayoutGranularity.GetInteger() )
#define IDVK_SHADER_INIT_SIZE ( r_vkContextShaderSize.GetInteger() )
#define IDVK_SHADER_GRANULARITY ( r_vkContextShaderGranularity.GetInteger() )
#define IDVK_SAMPLER_INIT_SIZE ( r_vkContextSamplerSize.GetInteger() )
#define IDVK_SAMPLER_GRANULARITY ( r_vkContextSamplerGranularity.GetInteger() )
#define IDVK_IMAGE_INIT_SIZE ( r_vkContextImageSize.GetInteger() )
#define IDVK_IMAGE_GRANULARITY ( r_vkContextImageGranularity.GetInteger() )
#define IDVK_BUFFER_INIT_SIZE ( r_vkContextBufferSize.GetInteger() )
#define IDVK_BUFFER_GRANULARITY ( r_vkContextBufferGranularity.GetInteger() )
#define IDVK_COMMANDPOOL_INIT_SIZE 1
#define IDVK_COMMANDPOOL_GRANULARITY 1
#define IDVK_FENCE_INIT_SIZE 2
#define IDVK_FENCE_GRANULARITY 2
#define IDVK_RENDERPASS_INIT_SIZE 1
#define IDVK_RENDERPASS_GRANULARITY 1
#define IDVK_QUERYPOOL_INIT_SIZE 1
#define IDVK_QUERYPOOL_GRANULARITY 1
#define IDVK_DESCRIPTORPOOL_INIT_SIZE 1
#define IDVK_DESCRIPTORPOOL_GRANULARITY 1

#else

#define IDVK_PILELINE_INIT_SIZE 64
#define IDVK_PIPELINE_GRANULARITY 64
#define IDVK_PIPELINE_LAYOUT_INIT_SIZE 64
#define IDVK_PIPELINE_LAYOUT_GRANULARITY 64
#define IDVK_DESCRIPTOR_POOL_SIZE_UBO (IDVK_MAX_DESCRIPTOR_POOL_SETS * 2) // vertex shader + fragment shader
#define IDVK_DESCRIPTOR_POOL_SIZE_SAMPLER (IDVK_MAX_DESCRIPTOR_POOL_SETS * 8) // max texture units in fragment shader
#define IDVK_SHADER_INIT_SIZE 128
#define IDVK_SHADER_GRANULARITY 128
#define IDVK_SAMPLER_INIT_SIZE 128
#define IDVK_SAMPLER_GRANULARITY 128
#define IDVK_IMAGE_INIT_SIZE 2048
#define IDVK_IMAGE_GRANULARITY 2048
#define IDVK_BUFFER_INIT_SIZE 2048
#define IDVK_BUFFER_GRANULARITY 2048
#define IDVK_COMMANDPOOL_INIT_SIZE 1
#define IDVK_COMMANDPOOL_GRANULARITY 1
#define IDVK_FENCE_INIT_SIZE 2
#define IDVK_FENCE_GRANULARITY 1
#define IDVK_RENDERPASS_INIT_SIZE 1
#define IDVK_RENDERPASS_GRANULARITY 1
#define IDVK_QUERYPOOL_INIT_SIZE 1
#define IDVK_QUERYPOOL_GRANULARITY 1
#define IDVK_DESCRIPTORPOOL_INIT_SIZE 1
#define IDVK_DESCRIPTORPOOL_GRANULARITY 1

#endif

#define currentSwapchainImage swapchain.framebuffers[vk_context.swapchain.imageIndex].image
#define currentFrameImageAvailableSemaphore imageAvailableSemaphores[vk_context.frameIndex]
#define currentFrameRenderFinishedSemaphore renderFinishedSemaphores[vk_context.frameIndex]
#define currentFrameInFlightFence inFlightFences[vk_context.frameIndex]
#define currentDescriptorPool descriptorPools[vk_context.frameIndex]

#define IDVK_CHECK_RENDERPASS_IDLE() assert(!vk_context.inRenderPass)

int windowWidth = 0;
int windowHeight = 0;

void *nativeWindow;

#ifdef _WIN32
typedef HWND WindowHandle_t;
#else
typedef void * WindowHandle_t;
#define _strdup strdup
#endif

VKContext_t vk_context;

#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
static void VMA_CreatrAlloctor()
{
    VmaAllocatorCreateInfo allocatorInfo;
	VKZERO(allocatorInfo);
    allocatorInfo.physicalDevice = vk_context.physicalDevice;
    allocatorInfo.device = vk_context.device;

    vmaCreateAllocator(&allocatorInfo, &vk_context.vmaAllocator);
}

static void VMA_DestroyAlloctor()
{
    vmaDestroyAllocator(vk_context.vmaAllocator);
}
#endif

/**
 * 加载全部Vulkan函数符号地址
 */
void VK_PreInit()
{
	volkInitialize();

    if(!vkCreateInstance)
    {
        Sys_Error("Can't load Vulkan symbols by Volk");
    }
}

/**
 * 枚举Vulkan实例的扩展
 * @return 扩展数量
 */
int VK_QueryExtensions()
{
    uint32_t num;
    VkResult result = vkEnumerateInstanceExtensionProperties(NULL, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumerateInstanceExtensionProperties");
    Sys_Printf("Enumerate Vulkan extensions: %d\n", num);

    vk_context.numExtensions = num;
    if(num > 0)
    {
        VkExtensionProperties *props = (VkExtensionProperties *)malloc(num * sizeof(props[0]));
        vkEnumerateInstanceExtensionProperties(NULL, &num, props);
        VKARRAY_ALLOC2(char *, vk_context.extensions, vk_context.numExtensions);
        for (int i = 0; i < num; ++i) {
            VkExtensionProperties *prop = props + i;
            vk_context.extensions[i] = _strdup(prop->extensionName);
            Sys_Printf("%3d: %s(%x)\n", i + 1, prop->extensionName, prop->specVersion);
        }
        free(props);
    }

    return (int)num;
}

/**
 * 获取物理设备属性和特性
 * @param physicalDevice 物理设备
 */
void VK_GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;

    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);

    Sys_Printf("Physics device properties: Api version=%x, Driver version=%x, Vendor ID=%x, Device ID=%x, Device type=%s, Device name=%s\n",
            physicalDeviceProperties.apiVersion,
            physicalDeviceProperties.driverVersion,
            physicalDeviceProperties.vendorID,
            physicalDeviceProperties.deviceID,
            VK_VkPhysicalDeviceTypeToName(physicalDeviceProperties.deviceType),
            physicalDeviceProperties.deviceName
            );

#define SUPPORT_BOOL(x, name) Sys_Printf(name "......%s\n", physicalDeviceFeatures.x ? "support" : "missing");
    Sys_Printf("Check features: \n");
    SUPPORT_BOOL(depthBounds, "Depth bounds");
    SUPPORT_BOOL(textureCompressionETC2, "ETC2 compression texture");
    SUPPORT_BOOL(textureCompressionBC, "BC compression texture");
    SUPPORT_BOOL(tessellationShader, "Tessellation shader");
    SUPPORT_BOOL(samplerAnisotropy, "Anisotropy sampler");
    SUPPORT_BOOL(geometryShader, "Geometry shader");
    SUPPORT_BOOL(tessellationShader, "Tessellation shader");

    Sys_Printf("\n");
    Sys_Printf("Check shader features: \n");
    SUPPORT_BOOL(shaderClipDistance, "Clip distance");
    SUPPORT_BOOL(shaderCullDistance, "Cull distance");
    SUPPORT_BOOL(shaderFloat64, "64bit-float");
#undef SUPPORT_BOOL

#define SUPPORT_LIMIT(x, name, fmt) Sys_Printf(name " = " fmt "\n", physicalDeviceProperties.limits.x);
    Sys_Printf("\n");
    Sys_Printf("Check limits: \n");
    SUPPORT_LIMIT(maxImageDimension2D, "Maximum 2D image dimension", "%u");
    SUPPORT_LIMIT(maxImageDimension3D, "Maximum 3D image dimension", "%u");
    SUPPORT_LIMIT(maxImageDimensionCube, "Maximum cube image dimension", "%u");
    SUPPORT_LIMIT(maxImageArrayLayers, "Maximum image array layers", "%u");
    SUPPORT_LIMIT(maxMemoryAllocationCount, "Maximum memory allocation count", "%u");
    SUPPORT_LIMIT(maxSamplerAllocationCount, "Maximum sampler allocation count", "%u");
    SUPPORT_LIMIT(maxBoundDescriptorSets, "Maximum bound descriptor sets", "%u");
    SUPPORT_LIMIT(maxColorAttachments, "Maximum color attachments", "%u");
    SUPPORT_LIMIT(maxSamplerAnisotropy, "Maximum sampler anisotropy", "%f");
    SUPPORT_LIMIT(minMemoryMapAlignment, "Minimum memory alignment", "%zu");
    SUPPORT_LIMIT(minTexelBufferOffsetAlignment, "Minimum texel buffer offset alignment", "%llu");
    SUPPORT_LIMIT(minUniformBufferOffsetAlignment, "Minimum uniform buffer offset alignment", "%llu");
    SUPPORT_LIMIT(minStorageBufferOffsetAlignment, "Minimum storage buffer offset alignment", "%llu");
    SUPPORT_LIMIT(optimalBufferCopyOffsetAlignment, "Optimal buffer copy offset alignment", "%llu");
    SUPPORT_LIMIT(optimalBufferCopyRowPitchAlignment, "Optimal buffer copy row pitch alignment", "%llu");
#undef SUPPORT_LIMIT
    Sys_Printf("Maximum framebuffer size = %u x %u x %u\n", physicalDeviceProperties.limits.maxFramebufferWidth, physicalDeviceProperties.limits.maxFramebufferHeight, physicalDeviceProperties.limits.maxFramebufferLayers);
    Sys_Printf("Point size range = %f - %f, granularity = %f\n", physicalDeviceProperties.limits.pointSizeRange[0], physicalDeviceProperties.limits.pointSizeRange[1], physicalDeviceProperties.limits.pointSizeGranularity);
    Sys_Printf("Line width range = %f - %f, granularity = %f\n", physicalDeviceProperties.limits.lineWidthRange[0], physicalDeviceProperties.limits.lineWidthRange[1], physicalDeviceProperties.limits.lineWidthGranularity);

    vk_context.physicalDeviceProperties = physicalDeviceProperties;
    vk_context.physicalDeviceFeatures = physicalDeviceFeatures;
}

/**
 * 枚举Vulkan实例的物理设备
 * @return 物理设备数量
 */
int VK_QueryPhysicalDevices()
{
    uint32_t num;
    VkResult result = vkEnumeratePhysicalDevices(vk_context.instance, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumeratePhysicalDevices");
    Sys_Printf("Enumerate Vulkan physical devices: %d\n", num);

    vk_context.numPhysicalDevices = num;
    if(num > 0)
    {
        VKARRAY_ALLOC2(VkPhysicalDevice, vk_context.physicalDevices, vk_context.numPhysicalDevices);
        vkEnumeratePhysicalDevices(vk_context.instance, &num, vk_context.physicalDevices);
        for (int i = 0; i < num; ++i) {
            Sys_Printf("%3d: %p\n", i + 1, vk_context.physicalDevices[i]);
            VK_GetPhysicalDeviceInfo(vk_context.physicalDevices[i]);
        }
    }

    return (int)num;
}

/**
 * 枚举Vulkan实例的验证层
 * @return 验证层数量
 */
int VK_QueryValidationLayers()
{
    uint32_t num;
    VkResult result = vkEnumerateInstanceLayerProperties(&num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumerateInstanceLayerProperties");
    Sys_Printf("Enumerate Vulkan validation layers: %d\n", num);

    vk_context.numValidationLayers = num;
    if(num > 0)
    {
        VkLayerProperties *supportValidationLayers = (VkLayerProperties *)malloc(num * sizeof(supportValidationLayers[0]));
        vkEnumerateInstanceLayerProperties(&num, supportValidationLayers);
        VKARRAY_ALLOC2(char *, vk_context.validationLayers, vk_context.numValidationLayers);
        for (int i = 0; i < num; ++i) {
            VkLayerProperties *layer = supportValidationLayers + i;
            Sys_Printf("%3d: %s: %s\n", i + 1, layer->layerName, layer->description);
            vk_context.validationLayers[i] = _strdup(layer->layerName);
        }
    }

    return (int)num;
}

/**
 * 枚举物理设备的验证层
 * @param pd 物理设备
 * @param ret 返回验证层名称列表
 * @return 验证层数量
 */
int VK_QueryPhysicalDeviceValidationLayers(VkPhysicalDevice pd, char ***ret)
{
    uint32_t num;
    VkResult result = vkEnumerateDeviceLayerProperties(pd, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumerateDeviceLayerProperties");
    Sys_Printf("Enumerate Vulkan device validation layers: %d\n", num);

    if(num <= 0)
        return 0;

    if(ret)
    {
        VkLayerProperties *supportValidationLayers = (VkLayerProperties *)malloc(num * sizeof(supportValidationLayers[0]));
        vkEnumerateDeviceLayerProperties(pd, &num, supportValidationLayers);
        *ret = (char **)malloc(num * sizeof((*ret)[0]));
        for (int i = 0; i < num; ++i) {
            VkLayerProperties *layer = supportValidationLayers + i;
            (*ret)[i] = _strdup(layer->layerName);
            Sys_Printf("%3d: %s: %s\n", i + 1, layer->layerName, layer->description);
        }
        free(supportValidationLayers);
    }

    return (int)num;
}

/**
 * 枚举物理设备的扩展
 * @param pd 物理设备
 * @param ret 返回扩展的名称列表
 * @return 扩展数量
 */
int VK_QueryPhysicalDeviceExtensions(VkPhysicalDevice pd, char ***ret)
{
    uint32_t num;
    VkResult result = vkEnumerateDeviceExtensionProperties(pd, NULL, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumerateDeviceExtensionProperties");
    Sys_Printf("Enumerate Vulkan device extensions: %d\n", num);

    if(num <= 0)
        return 0;

    if(ret)
    {
        VkExtensionProperties *supportExtensions = (VkExtensionProperties *)malloc(num * sizeof(supportExtensions[0]));
        vkEnumerateDeviceExtensionProperties(pd, NULL, &num, supportExtensions);
        *ret = (char **)malloc(num * sizeof((*ret)[0]));
        for (int i = 0; i < num; ++i) {
            VkExtensionProperties *extension = supportExtensions + i;
            (*ret)[i] = _strdup(extension->extensionName);
            Sys_Printf("%3d: %s(%u)\n", i, extension->extensionName, extension->specVersion);
        }
        free(supportExtensions);
    }

    return (int)num;
}

/**
 * 是否支持验证层
 */
bool VK_CheckValidationLayerSupported(const char *name)
{
    if (!vk_context.numValidationLayers)
        return false;

    for (int i = 0; i < vk_context.numValidationLayers; i++)
    {
        if (strcmp(vk_context.validationLayers[i], name) == 0)
            return true;
    }
    return false;
}

static int VK_EnableValidationLayers(char *total[], int length, const char ***ret)
{
    if (length <= 0)
        return 0;

    const char *EnabledValidationLayers[] = {
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_LUNARG_crash_diagnostic",
        "VK_LAYER_LUNARG_monitor",
    };
    int usedValidationLayerCount = 0;
    const char **usedValidationLayers = NULL;

#define USING_VALIDATION_LAYER(x) \
        if (!usedValidationLayers) \
            usedValidationLayers = (const char **)malloc(sizeof(*usedValidationLayers) * length); \
        usedValidationLayers[usedValidationLayerCount++] = x;

    for (int i = 0; i < length; i++)
    {
        for (int m = 0; m < COUNT_OF(EnabledValidationLayers); m++)
        {
            if (idStr::Icmp(total[i], EnabledValidationLayers[m]) == 0)
            {
                USING_VALIDATION_LAYER(EnabledValidationLayers[m]);
                break;
            }
        }
    }
#undef USING_VALIDATION_LAYER

    if (usedValidationLayerCount > 0)
        *ret = usedValidationLayers;

    return usedValidationLayerCount;
}

/**
 * 创建Vulkan实例
 */
void VK_CreateInstance()
{
    VkResult result;
    uint32_t apiVersion;

    result = vkEnumerateInstanceVersion(&apiVersion);
    HANDLE_VK_ERROR_EXIT(result, "vkEnumerateInstanceVersion");

    Sys_Printf("Enumerate Vulkan version: %d.%d.%d\n", VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion));

    VkApplicationInfo applicationInfo;
    VKZERO(applicationInfo);
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pEngineName = "idTech4A"; // 引擎名称
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // 引擎版本
    applicationInfo.pApplicationName = GAME_NAME; // 程序名称
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // 程序版本
    applicationInfo.apiVersion = apiVersion; // Vulkan API版本

    const char **usedValidationLayers = NULL;
    int usedValidationLayerCount = 0;
    if (r_vkEnableValidationLayer.GetBool())
        usedValidationLayerCount = VK_EnableValidationLayers(vk_context.validationLayers, vk_context.numValidationLayers, &usedValidationLayers);

    VkInstanceCreateInfo instanceCreateInfo;
    VKZERO(instanceCreateInfo);
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo; // 应用信息
    instanceCreateInfo.ppEnabledLayerNames = usedValidationLayers; // 启用的验证层数组
    instanceCreateInfo.enabledLayerCount = usedValidationLayerCount; // 启用的验证层数量
    instanceCreateInfo.ppEnabledExtensionNames = vk_context.extensions; // 启用的扩展数据
    instanceCreateInfo.enabledExtensionCount = vk_context.numExtensions; // 启用的扩展数量

    result = vkCreateInstance(&instanceCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.instance);
    free(usedValidationLayers);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateInstance");

    //Sys_Printf("vkInstance: %p\n", vk_context.instance);

	volkLoadInstance(vk_context.instance);
}

void VK_SetWindowNativeID(void *window)
{
    nativeWindow = window;
}

void VK_SetWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

/**
 * 创建渲染图层
 * @param window 本地窗口对象
 */
void VK_CreateSurface(void *window)
{
    VkResult result;

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    VKZERO(surfaceCreateInfo);
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hwnd = (WindowHandle_t)window;
    surfaceCreateInfo.hinstance = GetModuleHandle(NULL);

    result = vkCreateWin32SurfaceKHR(vk_context.instance, &surfaceCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.surface);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateWin32SurfaceKHR");
#else
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
    VKZERO(surfaceCreateInfo);
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.window = (ANativeWindow *)window; // pointer of ANativeWindow
	
	result = vkCreateAndroidSurfaceKHR(vk_context.instance, &surfaceCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.surface);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateAndroidSurfaceKHR");
#endif
}

/**
 * 获取物理设备的队列族
 * @param pd 物理设备
 * @param ret 返回队列族索引数组
 * @return 队列族数量
 */
int VK_QueryPhysicalDeviceQueueFamily(VkPhysicalDevice pd, uint32_t **ret)
{
    uint32_t num;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &num, NULL);
    Sys_Printf("Enumerate Vulkan physics device queue family: %d\n", num);

    if(num <= 0)
        return 0;

    VkQueueFamilyProperties *queueFamilyProperties = (VkQueueFamilyProperties *)malloc(num * sizeof(queueFamilyProperties[0]));
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &num, queueFamilyProperties);
    int res = 0;
    for (int i = 0; i < num; ++i) {
        VkQueueFamilyProperties *queueFamily = queueFamilyProperties + i;
        Sys_Printf("%3d: queue count=%d, flags=%s\n", i + 1, queueFamily->queueCount, VK_VkQueueFlagsBitToName(queueFamily->queueFlags));
        if(queueFamily->queueCount > 0 && queueFamily->queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            res++;
        }
    }

    if(!res)
    {
        free(queueFamilyProperties);
        return 0;
    }

    if(ret)
    {
        *ret = (uint32_t *)malloc(res * sizeof(uint32_t));
        uint32_t *ptr = *ret;
        for (int i = 0; i < num; ++i) {
            VkQueueFamilyProperties *queueFamily = queueFamilyProperties + i;
            if(queueFamily->queueCount > 0 && queueFamily->queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                *ptr = i;
                ptr++;
            }
        }
    }
    free(queueFamilyProperties);

    return res;
}

/**
 * 获取物理设备图形队列族索引
 * @param pd 物理设备
 * @return 队列族索引
 */
int VK_QueryPhysicalDeviceGraphicsQueueFamilyIndex(VkPhysicalDevice pd, int mask)
{
    uint32_t num;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &num, NULL);
    Sys_Printf("Enumerate Vulkan device queue family: %d\n", num);

    if(num <= 0)
        return -1;

    VkQueueFamilyProperties *queueFamilyProperties = (VkQueueFamilyProperties *)malloc(num * sizeof(queueFamilyProperties[0]));
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &num, queueFamilyProperties);
    int res = 0;
    for (int i = 0; i < num; ++i) {
        VkQueueFamilyProperties *queueFamily = queueFamilyProperties + i;
        if(queueFamily->queueCount > 0 && (queueFamily->queueFlags & mask) == mask)
        {
            return i;
        }
    }
    return -2;
}

/**
 * 检查物理设备是否支持
 * @param pd 物理设备
 * @return bool
 */
bool VK_CheckPhysicalDevice(VkPhysicalDevice pd)
{
    const char *requireExtensions[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    char **deviceExtensions = NULL;
    int num = VK_QueryPhysicalDeviceExtensions(pd, &deviceExtensions);
    if(num <= 0)
    {
        Sys_Printf("Missing Vulkan device extensions!\n");
        return false;
    }

    const int count = COUNT_OF(requireExtensions);
    int hasExt = 0;
    for (int i = 0; i < num; ++i) {
        int res = 0;
        for (int m = 0; m < count; ++m) {
            if(!strcmp(deviceExtensions[i], requireExtensions[m]))
                res++;
        }
        if(res == count)
        {
            hasExt = 1;
            break;
        }
    }

    bool ret = false;
    if(hasExt)
    {
        num = VK_QueryPhysicalDeviceQueueFamily(pd, NULL);
        if(num > 0)
        {
            ret = true;
        }
    }
    free(deviceExtensions);

    return ret;
}

/**
 * 选择物理设备
 */
void VK_ChoosePhysicalDevice()
{
    for (int i = 0; i < vk_context.numPhysicalDevices; ++i) {
        if(VK_CheckPhysicalDevice(vk_context.physicalDevices[i]))
        {
            vk_context.physicalDevice = vk_context.physicalDevices[i];
            break;
        }
    }
    if(vk_context.physicalDevice == VK_NULL_HANDLE)
    {
        Sys_Error("Choose null physical device!\n");
        return;
    }

    vk_context.numPhysicalExtensions = VK_QueryPhysicalDeviceExtensions(vk_context.physicalDevice, &vk_context.physicalExtensions);
    vk_context.numPhysicalValidationLayers = VK_QueryPhysicalDeviceValidationLayers(vk_context.physicalDevice, &vk_context.physicalValidationLayers);
}

/**
 * 获取物理设备图层格式
 * @param surfaceFormats 返回图层格式数组
 * @return 图层格式数量
 */
int VK_QueryPhysicalDeviceSurfaceFormats(VkSurfaceFormatKHR **surfaceFormats)
{
    uint32_t num;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(vk_context.physicalDevice, vk_context.surface, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    if(num > 0 && surfaceFormats)
    {
        *surfaceFormats = (VkSurfaceFormatKHR *)malloc(num * sizeof(surfaceFormats[0]));
        vkGetPhysicalDeviceSurfaceFormatsKHR(vk_context.physicalDevice, vk_context.surface, &num, *surfaceFormats);
    }

    return (int)num;
}

/**
 * 获取物理设置的呈现模式
 * @param presentModes 返回程序模式数组
 * @return 程序模式数量
 */
int VK_QueryPhysicalDevicePresentModes(VkPresentModeKHR **presentModes)
{
    uint32_t num;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(vk_context.physicalDevice, vk_context.surface, &num, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkGetPhysicalDeviceSurfacePresentModesKHR");
    if(num > 0 && presentModes)
    {
        *presentModes = (VkPresentModeKHR *)malloc(num * sizeof(presentModes[0]));
        vkGetPhysicalDeviceSurfacePresentModesKHR(vk_context.physicalDevice, vk_context.surface, &num, *presentModes);
    }

    return (int)num;
}

/**
 * 选择图层格式
 * @param ret 返回图层格式
 * @return 是否选择
 */
bool VK_ChooseSurfaceFormat(VkSurfaceFormatKHR *ret)
{
    VkSurfaceFormatKHR *surfaceFormats = NULL;
    int numFormats = VK_QueryPhysicalDeviceSurfaceFormats(&surfaceFormats);
	Sys_Printf("Surface supported formats: %d\n", numFormats);
    if(numFormats == 0)
        return false;

	for (int i = 0; i < numFormats; i++) {
		VkSurfaceFormatKHR *format = surfaceFormats + i;
		Sys_Printf("%d: format=%s, color space=%s\n", i + 1, VK_VkFormatToName(format->format), VK_VkColorSpaceKHRToName(format->colorSpace));
	}

	VkSurfaceFormatKHR targetFormat;
    VKZERO(targetFormat);
    if (numFormats == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        targetFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
        targetFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }
    else
    {
        VkSurfaceFormatKHR *found = NULL;
        for (int i = 0; i < numFormats; ++i) {
            VkSurfaceFormatKHR *format = surfaceFormats + i;
            if(format->format == VK_FORMAT_R8G8B8A8_UNORM && format->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                found = format;
                break;
            }
        }
        if(!found)
            found = &surfaceFormats[0];
        targetFormat = *found;
    }

    *ret = targetFormat;
    free(surfaceFormats);
    return true;
}

/**
 * 选择呈现模式
 * @param ret 返回呈现模式
 * @return 是否选择
 */
bool VK_ChoosePresentMode(VkPresentModeKHR *ret)
{
    VkPresentModeKHR *presentModes = NULL;
    int numModes = VK_QueryPhysicalDevicePresentModes(&presentModes);
    if(numModes == 0)
        return false;

    *ret = presentModes[0/*numModes - 1*/];
    free(presentModes);
    return true;
}

/**
 * 创建图像视图
 * @param image 图像
 * @param format 图像格式
 * @param imageViewType 2D
 * @param viewAspectFlags 格式
 * @param mipLevels mipmap层数
 * @param numLayers 图像层数
 * @return
 */
VkImageView VK_CreateImageView(VkImage image, int format, int imageViewType, int viewAspectFlags, int mipLevels, int numLayers)
{
	VkResult result;

    VkImageViewCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image; // 纹理图像
    createInfo.viewType = (VkImageViewType)imageViewType; // 纹理类型
    createInfo.format = (VkFormat)format; // 纹理数据格式

    // 纹理分量
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // R分量读取
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; // G分量读取
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; // B分量读取
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; // A分量读取

    createInfo.subresourceRange.aspectMask = (VkImageAspectFlags)viewAspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0; // 最基础的mipmap层
    createInfo.subresourceRange.levelCount = mipLevels; // mipmap层数
    createInfo.subresourceRange.baseArrayLayer = 0; // 图像起始层
    createInfo.subresourceRange.layerCount = numLayers; // 图像层数

    VkImageView imageView;
    result = vkCreateImageView(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &imageView);
    HANDLE_VK_ERROR_RETURNV(result, VK_NULL_HANDLE, "vkCreateImageView");

	return imageView;
}

/**
 * 创建交换链图像视图
 */
void VK_CreateSwapchainImageViews()
{
    for (int i = 0; i < vk_context.swapchain.numImages; ++i) {
        vk_context.swapchain.framebuffers[i].image.imageView = VK_CreateImageView(vk_context.swapchain.framebuffers[i].image.image, vk_context.swapchain.format, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1);
        vk_context.swapchain.loadFramebuffers[i].image.imageView = vk_context.swapchain.framebuffers[i].image.imageView;
    }
}

/**
 * 获取交换链图像
 */
void VK_GetSwapchainImages()
{
    // 获取交换链图像数量
    uint32_t numImages;
    VkResult result = vkGetSwapchainImagesKHR(vk_context.device, vk_context.swapchain.swapchain, &numImages, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vkGetSwapchainImagesKHR");
    if(numImages == 0)
    {
        Sys_Error("vkGetSwapchainImagesKHR get 0 images.\n");
        return;
    }

    // 获取交换链图像
    VkImage *images;
    VKARRAY_ALLOC2(VkImage, images, numImages);
    vkGetSwapchainImagesKHR(vk_context.device, vk_context.swapchain.swapchain, &numImages, images);

    Sys_Printf("Get swapchain images: %d\n", numImages);
    vk_context.swapchain.numImages = numImages;
    VKARRAY_ALLOC2(VKFramebuffer_t, vk_context.swapchain.framebuffers, vk_context.swapchain.numImages);
    VKARRAY_ALLOC2(VKFramebuffer_t, vk_context.swapchain.loadFramebuffers, vk_context.swapchain.numImages);

	VKImage_t *image;
    for (int i = 0; i < numImages; ++i)
    {
        image = &vk_context.swapchain.framebuffers[i].image;
        vk_context.swapchain.framebuffers[i].image.image = images[i];
        vk_context.swapchain.loadFramebuffers[i].image.image = images[i];
		image->format = (VkFormat)vk_context.swapchain.format;
		image->width = vk_context.swapchain.width;
		image->height = vk_context.swapchain.height;
		image->mipLevels = 1;
		image->usage = IU_ATTACHMENT;
		image->layers = 1;
		image->cubeMap = false;
		image->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    VKARRAY_FREE2(VkImage, images, numImages);
}


/**
 * 创建交换链帧缓冲区
 */
void VK_CreateSwapchainFramebuffers()
{
	VkResult result;

    for (int i = 0; i < vk_context.swapchain.numImages; ++i) {
        VK_CreateImage(&vk_context.swapchain.framebuffers[i].depthStencilImage, vk_context.swapchain.width, vk_context.swapchain.height, VK_FORMAT_D24_UNORM_S8_UINT, false, 1, 1, IU_ATTACHMENT);
		vk_context.swapchain.framebuffers[i].depthStencilImage.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkImageView attachments[] = {
            vk_context.swapchain.framebuffers[i].image.imageView,
            vk_context.swapchain.framebuffers[i].depthStencilImage.imageView
		};

        VkFramebufferCreateInfo createInfo;
        VKZERO(createInfo);
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = vk_context.renderPass; // 渲染通道
        createInfo.attachmentCount = COUNT_OF(attachments); // 交换链帧缓冲区附件数量
        createInfo.pAttachments = attachments; // 交换链帧缓冲区附件
        createInfo.width = vk_context.swapchain.width; // 交换链宽度
        createInfo.height = vk_context.swapchain.height; // 交换链高度
        createInfo.layers = 1;

        result = vkCreateFramebuffer(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.swapchain.framebuffers[i].framebuffer);
        HANDLE_VK_ERROR_EXIT(result, "vkCreateFramebuffer");

        vk_context.swapchain.loadFramebuffers[i].depthStencilImage = vk_context.swapchain.framebuffers[i].depthStencilImage;
        createInfo.renderPass = vk_context.loadRenderPass; // 渲染通道
        result = vkCreateFramebuffer(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.swapchain.loadFramebuffers[i].framebuffer);
        HANDLE_VK_ERROR_EXIT(result, "vkCreateFramebuffer");
    }
}

/**
 * 创建交换链
 */
void VK_CreateSwapchain()
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_context.physicalDevice, vk_context.surface, &surfaceCapabilities);
    HANDLE_VK_ERROR_EXIT(result, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	vk_context.surfaceCapabilities = surfaceCapabilities;

    VkSurfaceFormatKHR targetFormat;
    if(!VK_ChooseSurfaceFormat(&targetFormat))
    {
        Sys_Error("vkGetPhysicalDeviceSurfaceFormatsKHR unable choose format.\n");
        return;
    }

    VkPresentModeKHR presentMode;
    if(!VK_ChoosePresentMode(&presentMode))
    {
        Sys_Error("vkGetPhysicalDeviceSurfacePresentModesKHR unable choose mode.\n");
        return;
    }

    vk_context.swapchain.width = windowWidth > 0 ? windowWidth : surfaceCapabilities.currentExtent.width;
    vk_context.swapchain.height = windowHeight > 0 ? windowHeight : surfaceCapabilities.currentExtent.height;
    vk_context.swapchain.format = targetFormat.format;
    vk_context.swapchain.colorSpace = targetFormat.colorSpace;
    uint32_t queueFamilyIndexs[] = {
            vk_context.graphicsQueue.familyIndex
    };

    VkSwapchainCreateInfoKHR createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vk_context.surface; // 图层
    createInfo.minImageCount = surfaceCapabilities.minImageCount; // 最少交换链图像数
    createInfo.imageFormat = targetFormat.format; // 交换链图像格式
    createInfo.imageColorSpace = targetFormat.colorSpace; // 交换链颜色空间
    createInfo.imageExtent.width = vk_context.swapchain.width; // 交换链图像宽度
    createInfo.imageExtent.height = vk_context.swapchain.height; // 交换链图像高度
    createInfo.imageArrayLayers = 1; // 交换链图像层数
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // 交换链图像用法

    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 1; // 队列簇数量
    createInfo.pQueueFamilyIndices = queueFamilyIndexs; // 队列簇索引数组

#ifdef __ANDROID__
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // 交换链方向
#else
    createInfo.preTransform = surfaceCapabilities.currentTransform; // 交换链方向
#endif
	if(surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	else
	    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

    createInfo.presentMode = presentMode; // 呈现模式
    createInfo.clipped = VK_TRUE; // 是否裁剪

    VkSwapchainKHR old = vk_context.swapchain.swapchain;
    createInfo.oldSwapchain = old; // 旧的交换链

    result = vkCreateSwapchainKHR(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.swapchain.swapchain);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateSwapchainKHR");

    Sys_Printf("Swapchain format: %s\n", VK_VkFormatToName(vk_context.swapchain.format));
    Sys_Printf("Swapchain color space: %s\n", VK_VkColorSpaceKHRToName(vk_context.swapchain.colorSpace));

    // 获取交换链图像
    VK_GetSwapchainImages();

    // 创建交换链图像视图
    VK_CreateSwapchainImageViews();
}

/**
 * 创建逻辑设备
 */
void VK_CreateDevice()
{
    uint32_t *queueFamily = NULL;
    int num = VK_QueryPhysicalDeviceQueueFamily(vk_context.physicalDevice, &queueFamily);
    float queuePriorities = 1.0f;

    VkDeviceQueueCreateInfo *deviceQueueCreateInfos = (VkDeviceQueueCreateInfo *)malloc(num * sizeof(deviceQueueCreateInfos[0]));
    for (int i = 0; i < num; ++i) {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        VKZERO(deviceQueueCreateInfo);
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueCount = 1; // 队列数量
        deviceQueueCreateInfo.pQueuePriorities = &queuePriorities; // 队列优先级
        deviceQueueCreateInfo.queueFamilyIndex = queueFamily[i]; // 队列簇索引
        deviceQueueCreateInfos[i] = deviceQueueCreateInfo;
    }

    const char **usedValidationLayers = NULL;
    int usedValidationLayerCount = 0;
    if (r_vkEnableValidationLayer.GetBool())
        usedValidationLayerCount = VK_EnableValidationLayers(vk_context.physicalValidationLayers, vk_context.numPhysicalValidationLayers, &usedValidationLayers);

    VkPhysicalDeviceFeatures enabledFeatures;
    VKZERO(enabledFeatures);
    enabledFeatures.wideLines = vk_context.physicalDeviceFeatures.wideLines; // 允许修改 > 1.0的线宽度
    enabledFeatures.samplerAnisotropy = vk_context.physicalDeviceFeatures.samplerAnisotropy;
    enabledFeatures.textureCompressionETC2 = vk_context.physicalDeviceFeatures.textureCompressionETC2;
    enabledFeatures.fillModeNonSolid = vk_context.physicalDeviceFeatures.fillModeNonSolid;
    enabledFeatures.occlusionQueryPrecise = vk_context.physicalDeviceFeatures.occlusionQueryPrecise;

    VkDeviceCreateInfo deviceCreateInfo;
    VKZERO(deviceCreateInfo);
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.enabledExtensionCount = vk_context.numPhysicalExtensions; // 逻辑设备启用的扩展数量
    deviceCreateInfo.ppEnabledExtensionNames = vk_context.physicalExtensions; // 逻辑设备启用的扩展数组
    deviceCreateInfo.enabledLayerCount = usedValidationLayerCount; // 逻辑设备启用的验证层数量
    deviceCreateInfo.ppEnabledLayerNames = usedValidationLayers; // 逻辑设备启用的验证层数组
    deviceCreateInfo.queueCreateInfoCount = num; // 逻辑设备启用的队列数量
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos; // 逻辑设备启用的队列数组
    deviceCreateInfo.pEnabledFeatures = &enabledFeatures; // 启用的支持

    VkResult result = vkCreateDevice(vk_context.physicalDevice, &deviceCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.device);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateDevice");

    vk_context.graphicsQueue.familyIndex = VK_QueryPhysicalDeviceGraphicsQueueFamilyIndex(vk_context.physicalDevice, VK_QUEUE_GRAPHICS_BIT);
    int index = VK_QueryPhysicalDeviceGraphicsQueueFamilyIndex(vk_context.physicalDevice, VK_QUEUE_TRANSFER_BIT);
    if (index >= 0)
        vk_context.transferQueue.familyIndex = index;
    else
        vk_context.transferQueue.familyIndex = vk_context.graphicsQueue.familyIndex;

    vkGetDeviceQueue(vk_context.device, vk_context.graphicsQueue.familyIndex, 0, &vk_context.graphicsQueue.queue);
    if (vk_context.graphicsQueue.familyIndex != vk_context.transferQueue.familyIndex)
        vkGetDeviceQueue(vk_context.device, vk_context.transferQueue.familyIndex, 0, &vk_context.transferQueue.queue);
    else
        vk_context.transferQueue.queue = vk_context.graphicsQueue.queue;

    free(queueFamily);
    free(usedValidationLayers);
}

/**
 * 创建着色器模块
 * @param data spv着色器数据
 * @param length 数据长度
 * @return 着色器模块
 */
VkShaderModule VK_CreateShaderModule(const uint8_t *data, int length)
{
    VkShaderModuleCreateInfo createInfo;
    VKZERO(createInfo);
    VkShaderModule shaderModule;

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = length; // 着色器源码大小
    createInfo.pCode = (uint32_t *)data; // 着色器源码数据

    vkCreateShaderModule(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &shaderModule);

    return shaderModule;
}

static void VK_CreateLoadRenderPass()
{
    VkAttachmentDescription attachments[2];
    VKZERO(attachments);
    // 颜色附件
    attachments[0].format = vk_context.swapchain.format; // 颜色图像格式
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT; // 采样数
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // 颜色附件加载时执行操作
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 颜色附件存储时的操作
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 模版附件加载时执行操作
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // 模版附件存储时执行操作
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // 交换链图像附件初始布局
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 交换链图像附件最终布局

    // 深度+模版附件
    attachments[1].format = VK_FORMAT_D24_UNORM_S8_UINT; // 深度图像格式
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT; // 采样数
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // 深度附件加载时执行操作
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 深度附件存储时的操作
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // 模版附件加载时执行操作
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE; // 模版附件存储时执行操作
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // 交换链图像附件初始布局
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 交换链图像附件最终布局

    // 图像附件
    VkAttachmentReference colorAttachmentRef;
    VKZERO(colorAttachmentRef);
    colorAttachmentRef.attachment = 0; // 颜色附件索引
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // 深度+模版附件
    VkAttachmentReference depthStencilAttachmentRef;
    VKZERO(depthStencilAttachmentRef);
    depthStencilAttachmentRef.attachment = 1; // 深度附件索引
    depthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // 子通道
    VkSubpassDescription subpassDescription;
    VKZERO(subpassDescription);
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1; // 颜色附件数
    subpassDescription.pColorAttachments = &colorAttachmentRef; // 颜色附件
    subpassDescription.pDepthStencilAttachment = &depthStencilAttachmentRef; // 深度模版附件

    // 子通道阶段依赖
    VkSubpassDependency subpassDependencys[2];
	VKZERO(subpassDependencys);
    // 颜色通道
    subpassDependencys[0].srcSubpass = VK_SUBPASS_EXTERNAL; // 当前从子通道外部进入
    subpassDependencys[0].dstSubpass = 0; // 进入子通道0
    subpassDependencys[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependencys[0].srcAccessMask = 0;
    subpassDependencys[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependencys[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // 深度通道
    subpassDependencys[1].srcSubpass = 0; // 当前从子通道0进入
    subpassDependencys[1].dstSubpass = VK_SUBPASS_EXTERNAL; // 进入子通道外部
    subpassDependencys[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    subpassDependencys[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    subpassDependencys[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // 管线结束
    subpassDependencys[1].dstAccessMask = 0;

    VkRenderPassCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = COUNT_OF(attachments); // 渲染通道附件数
    createInfo.pAttachments = attachments; // 渲染通道附件
    createInfo.dependencyCount = COUNT_OF(subpassDependencys); // 渲染子通道依赖数
    createInfo.pDependencies = subpassDependencys; // 渲染子通道依赖
    createInfo.subpassCount = 1; // 渲染子通道数量
    createInfo.pSubpasses = &subpassDescription; // 渲染子通道

    VkResult result = vkCreateRenderPass(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.loadRenderPass);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateRenderPass");
}

/**
 * 创建渲染通道
 */
void VK_CreateRenderPass()
{
    VkAttachmentDescription attachments[2];
    VKZERO(attachments);
    // 颜色附件
    attachments[0].format = vk_context.swapchain.format; // 颜色图像格式
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT; // 采样数
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 颜色附件加载时执行操作
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 颜色附件存储时的操作
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 模版附件加载时执行操作
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // 模版附件存储时执行操作
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // 交换链图像附件初始布局
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 交换链图像附件最终布局

    // 深度+模版附件
    attachments[1].format = VK_FORMAT_D24_UNORM_S8_UINT; // 深度图像格式
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT; // 采样数
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 深度附件加载时执行操作
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 深度附件存储时的操作
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 模版附件加载时执行操作
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE; // 模版附件存储时执行操作
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // 交换链图像附件初始布局
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 交换链图像附件最终布局

    // 图像附件
    VkAttachmentReference colorAttachmentRef;
    VKZERO(colorAttachmentRef);
    colorAttachmentRef.attachment = 0; // 颜色附件索引
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // 深度+模版附件
    VkAttachmentReference depthStencilAttachmentRef;
    VKZERO(depthStencilAttachmentRef);
    depthStencilAttachmentRef.attachment = 1; // 深度附件索引
    depthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // 子通道
    VkSubpassDescription subpassDescription;
    VKZERO(subpassDescription);
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1; // 颜色附件数
    subpassDescription.pColorAttachments = &colorAttachmentRef; // 颜色附件
    subpassDescription.pDepthStencilAttachment = &depthStencilAttachmentRef; // 深度模版附件

    // 子通道阶段依赖
    VkSubpassDependency subpassDependencys[2];
	VKZERO(subpassDependencys);
    // 颜色通道
    subpassDependencys[0].srcSubpass = VK_SUBPASS_EXTERNAL; // 当前从子通道外部进入
    subpassDependencys[0].dstSubpass = 0; // 进入子通道0
    subpassDependencys[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependencys[0].srcAccessMask = 0;
    subpassDependencys[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependencys[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // 深度通道
    subpassDependencys[1].srcSubpass = 0; // 当前从子通道0进入
    subpassDependencys[1].dstSubpass = VK_SUBPASS_EXTERNAL; // 进入子通道外部
    subpassDependencys[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    subpassDependencys[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    subpassDependencys[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // 管线结束
    subpassDependencys[1].dstAccessMask = 0;

    VkRenderPassCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = COUNT_OF(attachments); // 渲染通道附件数
    createInfo.pAttachments = attachments; // 渲染通道附件
    createInfo.dependencyCount = COUNT_OF(subpassDependencys); // 渲染子通道依赖数
    createInfo.pDependencies = subpassDependencys; // 渲染子通道依赖
    createInfo.subpassCount = 1; // 渲染子通道数量
    createInfo.pSubpasses = &subpassDescription; // 渲染子通道

    VkResult result = vkCreateRenderPass(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.renderPass);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateRenderPass");

    // 创建加载阶段的渲染通道
    VK_CreateLoadRenderPass();
}

/**
 * 创建同步栅栏
 */
static void VK_CreateFence(VkFence *fence, bool signaled)
{
    VkResult result;

    VkFenceCreateInfo fenceCreateInfo;
    VKZERO(fenceCreateInfo);
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0; // 同步栅栏创建时为已发射状态

    result = vkCreateFence(vk_context.device, &fenceCreateInfo, VKCONTEXT_ALLOCATOR, fence);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateFence");
}

/**
 * 创建命令缓冲区
 */
static void VK_CreateCommandBuffers(VKCommandPool_t *commandPool)
{
    VkCommandBufferAllocateInfo allocateInfo;
    VKZERO(allocateInfo);
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool->commandPool; // 命令池
    allocateInfo.commandBufferCount = NUM_INFLIGHT_FRAME; // 命令缓冲区数量
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkResult result = vkAllocateCommandBuffers(vk_context.device, &allocateInfo, commandPool->commandBuffers);
    HANDLE_VK_ERROR_EXIT(result, "vkAllocateCommandBuffers");
}

/**
 * 创建命令池
 */
void VK_CreateCommandPool(VKCommandPool_t *commandPool, int usage, bool reset, bool createFence)
{
    VkCommandPoolCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = usage == CU_TRANSFER ? vk_context.transferQueue.familyIndex : vk_context.graphicsQueue.familyIndex; // 队列簇索引
    createInfo.flags = reset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;

    VkResult result = vkCreateCommandPool(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &commandPool->commandPool);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateCommandPool");

    VK_CreateCommandBuffers(commandPool);

    if (createFence)
    {
        for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
        {
            VK_CreateFence(&commandPool->fences[i], false);
        }
    }
}

/**
 * 创建主命令池
 */
static void VK_CreateCommands()
{
    VK_CreateCommandPool(&vk_context.commandPool, CU_PRESENT, true, false);
}

/**
 * 创建同步栅栏
 */
static void VK_CreateFences()
{
    VkResult result;

    VkFenceCreateInfo fenceCreateInfo;
    VKZERO(fenceCreateInfo);
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // 同步栅栏创建时为已发射状态

    for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
    {
        result = vkCreateFence(vk_context.device, &fenceCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.inFlightFences[i]);
        HANDLE_VK_ERROR_EXIT(result, "vkCreateFence");
    }
}

/**
 * 创建信号量
 */
static void VK_CreateSemaphores()
{
    VkResult result;

    VkSemaphoreCreateInfo semaphoreCreateInfo;
    VKZERO(semaphoreCreateInfo);
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		result = vkCreateSemaphore(vk_context.device, &semaphoreCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.imageAvailableSemaphores[i]);
		HANDLE_VK_ERROR_EXIT(result, "semaphoreCreateInfo");
		result = vkCreateSemaphore(vk_context.device, &semaphoreCreateInfo, VKCONTEXT_ALLOCATOR, &vk_context.renderFinishedSemaphores[i]);
		HANDLE_VK_ERROR_EXIT(result, "semaphoreCreateInfo");
	}
}

/**
 * 查询物理设备的内存类型
 * @param physicalDevice
 * @param memoryTypeBits
 * @param properties
 * @return
 */
int VK_QueryPhysicalDeviceMemoryType(VkPhysicalDevice physicalDevice, uint32_t memoryTypeBits, uint32_t properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (int i = 0; i < memProperties.memoryTypeCount; ++i) {
        if ((memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return -1;
}

void VK_CreateVertexBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush)
{
	VK_CreateBuffer(ret, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, size, map, flush);
}

void VK_CreateIndexBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush)
{
	VK_CreateBuffer(ret, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, size, map, flush);
}

void VK_CreateUniformBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush)
{
	VK_CreateBuffer(ret, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size, map, flush);
}

void VK_CreateTransferSrcBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush)
{
	VK_CreateBuffer(ret, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, size, map, flush);
}

void VK_CreateTransferDstBuffer(VKBuffer_t *ret, uint32_t size, bool map, bool flush)
{
    VK_CreateBuffer(ret, VK_BUFFER_USAGE_TRANSFER_DST_BIT, size, map, flush);
}

/**
 * 创建缓冲区
 * @param ret 缓冲区
 * @param usage 用途
 * @param bits 旗标
 * @param size 缓冲区大小
 * @param data 初始化数据
 */
void VK_CreateBuffer(VKBuffer_t *ret, int usage, uint32_t size, bool map, bool flush)
{
	VkMemoryPropertyFlags bits = 0;
	if(map)
	{
		bits |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		if(flush)
			bits |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	}
	else
	{
		bits |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}
	
    VkBufferCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size; // 缓冲区大小
    createInfo.usage = (VkBufferUsageFlags)usage; // 缓冲区用法
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    VmaMemoryRequirements memReq;
	VKZERO(memReq);
	memReq.requiredFlags = bits;
    memReq.usage = VMA_MEMORY_USAGE_UNKNOWN;
	if(map)
	{
		memReq.flags = VMA_MEMORY_REQUIREMENT_OWN_MEMORY_BIT;
		if(flush)
			memReq.usage = VMA_MEMORY_USAGE_CPU_ONLY;
		else
			memReq.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	}
	else
		memReq.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VmaAllocation allocation;
	VmaAllocationInfo allocationInfo;
    VkResult result = vmaCreateBuffer(vk_context.vmaAllocator, &createInfo, &memReq, &buffer, &allocation, &allocationInfo);
    HANDLE_VK_ERROR_EXIT(result, "vmaCreateBuffer");

    ret->vmaAllocation = allocation;
    ret->size = allocationInfo.size; // vmaGetAllocationSize(allocation);
    ret->alignment = allocationInfo.alignment; // vmaGetAllocationAlignment(allocation);
#else
    VkResult result = vkCreateBuffer(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &buffer);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateBuffer");

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vk_context.device, buffer, &memoryRequirements);

    int memoryTypeIndex = VK_QueryPhysicalDeviceMemoryType(vk_context.physicalDevice, memoryRequirements.memoryTypeBits, bits);
    VkMemoryAllocateInfo memoryAllocateInfo;
    VKZERO(memoryAllocateInfo);
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

    VkDeviceMemory memory;
    result = vkAllocateMemory(vk_context.device, &memoryAllocateInfo, VKCONTEXT_ALLOCATOR, &memory);
    HANDLE_VK_ERROR_EXIT(result, "vkAllocateMemory");

    result = vkBindBufferMemory(vk_context.device, buffer, memory, 0);
    HANDLE_VK_ERROR_EXIT(result, "vkBindBufferMemory");

    ret->memory = memory;
    ret->size = memoryRequirements.size;
    ret->alignment = memoryRequirements.alignment;
#endif

    ret->buffer = buffer;
	ret->map = bits & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	ret->flush = bits & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

void * VK_MapBuffer(VKBuffer_t *buffer, int offset, int size)
{
    void *data;
    VkResult result;

	assert(buffer->map);

    // 映射显存到内存
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	result = vmaMapMemory2(vk_context.vmaAllocator, buffer->vmaAllocation, offset, size, &data);
#else
	result = vkMapMemory(vk_context.device, buffer->memory, offset, size, 0, &data);
#endif
    HANDLE_VK_ERROR_EXIT(result, "vkMapMemory");

	return data;
}

void VK_UnmapBuffer(VKBuffer_t *buffer)
{
    // 解除显存映射
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    vmaUnmapMemory(vk_context.vmaAllocator, buffer->vmaAllocation);
#else
    vkUnmapMemory(vk_context.device, buffer->memory);
#endif
}

/**
 * 刷新缓冲区(CPU到GPU)
 * @param buffer 缓冲区
 * @param offset 偏移
 * @param size 大小
 */
void VK_FlushBuffer(VKBuffer_t *buffer, int offset, int size)
{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	VmaMappedMemoryRange mappedMemoryRange;
	VKZERO(mappedMemoryRange);

	mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedMemoryRange.offset = offset;
	mappedMemoryRange.size = size;
	mappedMemoryRange.allocation = buffer->vmaAllocation;

	vmaFlushMappedMemoryRanges(vk_context.vmaAllocator, 1, &mappedMemoryRange);
#else
	VkMappedMemoryRange mappedMemoryRange;
	VKZERO(mappedMemoryRange);

	mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedMemoryRange.offset = offset;
	mappedMemoryRange.size = size;
	mappedMemoryRange.memory = buffer->memory;

	vkFlushMappedMemoryRanges(vk_context.device, 1, &mappedMemoryRange);
#endif
}

/**
 * 同步缓冲区(GPU到CPU)
 * @param buffer 缓冲区
 * @param offset 偏移
 * @param size 大小
 */
void VK_InvalidateBuffer(VKBuffer_t *buffer, int offset, int size)
{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    VmaMappedMemoryRange mappedMemoryRange;
    VKZERO(mappedMemoryRange);

    mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedMemoryRange.offset = offset;
    mappedMemoryRange.size = size;
    mappedMemoryRange.allocation = buffer->vmaAllocation;

    vmaInvalidateMappedMemoryRanges(vk_context.vmaAllocator, 1, &mappedMemoryRange);
#else
    VkMappedMemoryRange mappedMemoryRange;
    VKZERO(mappedMemoryRange);

    mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedMemoryRange.offset = offset;
    mappedMemoryRange.size = size;
    mappedMemoryRange.memory = buffer->memory;

    vkInvalidateMappedMemoryRanges(vk_context.device, 1, &mappedMemoryRange);
#endif
}

/**
 * 复制CPU内存到缓冲区
 */
void VK_WriteBuffer(VKBuffer_t *buffer, const void *src, int offset, int size, void *ptr)
{
	assert(buffer->map);

	bool needMap = !ptr;
	if(needMap)
		ptr = VK_MapBuffer(buffer, offset, size);
	memcpy(ptr, src, size);
	if(!buffer->flush)
		VK_FlushBuffer(buffer, offset, size);
	if(needMap)
		VK_UnmapBuffer(buffer);
}

/**
 * 复制缓冲区到CPU内存
 */
void VK_ReadBuffer(VKBuffer_t *buffer, void *dst, int offset, int size, void *ptr)
{
    assert(buffer->map);

    bool needMap = !ptr;
    if(needMap)
        ptr = VK_MapBuffer(buffer, offset, size);
    memcpy(dst, ptr, size);
    if(!buffer->flush)
        VK_InvalidateBuffer(buffer, offset, size);
    if(needMap)
        VK_UnmapBuffer(buffer);
}

/**
 * 创建描述符池
 */
void VK_CreateDescriptorPool(VKDescriptorPool_t *descriptorPool)
{
    VkDescriptorPoolSize descriptorPoolSizes[2];
    descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // Uniform块
    descriptorPoolSizes[0].descriptorCount = IDVK_DESCRIPTOR_POOL_SIZE_UBO; // Uniform块数量
    descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // 图像采样器
    descriptorPoolSizes[1].descriptorCount = IDVK_DESCRIPTOR_POOL_SIZE_SAMPLER; // 图像采样器数量

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
    VKZERO(descriptorPoolCreateInfo);
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount = COUNT_OF(descriptorPoolSizes);
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes;
    descriptorPoolCreateInfo.maxSets = IDVK_MAX_DESCRIPTOR_POOL_SETS; // 最大描述符集数量

    VkResult result;

    result = vkCreateDescriptorPool(vk_context.device, &descriptorPoolCreateInfo, VKCONTEXT_ALLOCATOR, &descriptorPool->descriptorPool);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateDescriptorPool");
}

/**
 * 创建描述符池
 */
void VK_CreateDescriptorPools()
{
	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
	    VKQUEUE_ALLOC(VKDescriptorPool_t, vk_context.descriptorPools[i]);
		VK_CreateDescriptorPool(_vkc_value);
	}
}

/**
 * 创建描述符集
 */
VKHandle_t VK_CreateDescriptorSet(VkDescriptorSet **descriptorSet, VkDescriptorPool *descriptorPool, VkDescriptorSetLayout layouts[], int count, int numUniforms, int numSamplers)
{
    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
    VKZERO(descriptorSetAllocateInfo);
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = *descriptorPool; // 描述符池
    descriptorSetAllocateInfo.descriptorSetCount = count; // 分配描述符集数量
    descriptorSetAllocateInfo.pSetLayouts = layouts; // 每个描述符集布局

    VkResult result = vkAllocateDescriptorSets(vk_context.device, &descriptorSetAllocateInfo, *descriptorSet);
	if(result == VK_ERROR_OUT_OF_POOL_MEMORY)
	{
		VKQUEUE_FOREACH(VKDescriptorPool_t, vk_context.currentDescriptorPool)
		{
			if(*descriptorPool == vk_context.currentDescriptorPool.list[i].descriptorPool)
			{
				vk_context.currentDescriptorPool.list[i].usedDescriptorSets = IDVK_MAX_DESCRIPTOR_POOL_SETS;
				break;
			}
		}

		VKHandle_t handle = VK_AllocDescriptorSet(descriptorSet, descriptorPool, numUniforms, numSamplers);

		descriptorSetAllocateInfo.descriptorPool = *descriptorPool; // 描述符池
		result = vkAllocateDescriptorSets(vk_context.device, &descriptorSetAllocateInfo, *descriptorSet);
		return handle;
	}

    HANDLE_VK_ERROR_EXIT(result, "vkAllocateDescriptorSets");

	return VKHANDLE_INVALID;
}

void VK_BlitColorImage(VKImage_t *dstImage, const VKImage_t *srcImage)
{
    VkImageBlit imageBlit;
    VKZERO(imageBlit);
    imageBlit.srcOffsets[0].x = 0;
    imageBlit.srcOffsets[0].y = 0;
    imageBlit.srcOffsets[0].z = 0;
    imageBlit.srcOffsets[1].x = srcImage->width;
    imageBlit.srcOffsets[1].y = srcImage->height;
    imageBlit.srcOffsets[1].z = 1;
    imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageBlit.srcSubresource.mipLevel = 1;
    imageBlit.srcSubresource.baseArrayLayer = 0;
    imageBlit.srcSubresource.layerCount = 1;

    imageBlit.dstOffsets[0].x = 0;
    imageBlit.dstOffsets[0].y = 0;
    imageBlit.dstOffsets[0].z = 0;
    imageBlit.dstOffsets[1].x = dstImage->width;
    imageBlit.dstOffsets[1].y = dstImage->height;
    imageBlit.dstOffsets[1].z = 1;
    imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageBlit.dstSubresource.mipLevel = 1;
    imageBlit.dstSubresource.baseArrayLayer = 0;
    imageBlit.dstSubresource.layerCount = 1;

    vkCmdBlitImage(vk_context.currentFrameCommandBuffer, srcImage->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);
}

void VK_MakeSwapchainColorImageToCopy(VKImage_t *image)
{
	IDVK_CHECK_RENDERPASS_IDLE();

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    vkCmdPipelineBarrier(vk_context.currentFrameCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);

	image->layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
}

/**
 * 复制缓冲区到纹理
 * @param image 纹理
 * @param buffer 缓冲区
 */
void VK_CopyBufferToBuffer(const VKBuffer_t *dstBuffer, const VKBuffer_t *buffer, int dstOffset, int offset, int size, VkCommandBuffer cmd)
{
	IDVK_CHECK_RENDERPASS_IDLE();

	VkBufferCopy bufferCopy;
	VKZERO(bufferCopy);
	bufferCopy.srcOffset = offset;
	bufferCopy.dstOffset = dstOffset;
	bufferCopy.size = size;

	vkCmdCopyBuffer(cmd, buffer->buffer, dstBuffer->buffer, 1, &bufferCopy);
    
    VkBufferMemoryBarrier bufferMemoryBarrier;
    VKZERO(bufferMemoryBarrier);
    bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.buffer = buffer->buffer;
    bufferMemoryBarrier.offset = offset;
    bufferMemoryBarrier.size = size;
    bufferMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    bufferMemoryBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, NULL, 1, &bufferMemoryBarrier, 0, NULL);
}


/**
 * 复制缓冲区到纹理
 * @param image 纹理
 * @param buffer 缓冲区
 */
void VK_CopyBufferToImage(VKImage_t *image, const VKBuffer_t *buffer, int offset, int mipLevels, int sizes[], int offsets[], VkCommandBuffer cmd)
{
	IDVK_CHECK_RENDERPASS_IDLE();
	int layerCount = image->cubeMap ? 6 : 1;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = layerCount;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.oldLayout = image->layout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

	if(image->layout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_HOST_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
				NULL, 1, &imageMemoryBarrier);
		image->layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	}

    if (!sizes || !offsets) // (mipLevels == 1)
    {
        VkBufferImageCopy bufferImageCopy;
        VKZERO(bufferImageCopy);
        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = layerCount;
        bufferImageCopy.imageExtent.width = image->width;
        bufferImageCopy.imageExtent.height = image->height;
        bufferImageCopy.imageExtent.depth = 1;
        bufferImageCopy.bufferOffset = offset;

        vkCmdCopyBufferToImage(cmd, buffer->buffer, image->image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &bufferImageCopy);
    }
    else
    {
		VkBufferImageCopy *bufferImageCopy = (VkBufferImageCopy *)malloc(sizeof(*bufferImageCopy) * mipLevels);
        for(int i = 0; i < mipLevels; i++)
        {
            VKZERO(bufferImageCopy[i]);
            bufferImageCopy[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferImageCopy[i].imageSubresource.mipLevel = i;
            bufferImageCopy[i].imageSubresource.baseArrayLayer = 0;
            bufferImageCopy[i].imageSubresource.layerCount = layerCount;
            bufferImageCopy[i].imageExtent.width = sizes[i * 2];
            bufferImageCopy[i].imageExtent.height = sizes[i * 2 + 1];
            bufferImageCopy[i].imageExtent.depth = 1;
            bufferImageCopy[i].bufferOffset = offset + offsets[i];
        }

		vkCmdCopyBufferToImage(cmd, buffer->buffer, image->image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				mipLevels, bufferImageCopy);
		free(bufferImageCopy);
    }

    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL,
                         0, NULL, 1, &imageMemoryBarrier);
	image->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

/**
 * 创建纹理图像(2D, 立方体, 2D数组)
 * @param res 图像
 * @param width 宽度
 * @param height 高度
 * @param format 格式(VkFormat)
 * @param cubeMap 是否是立方体纹理
 * @param layers 纹理数组大小
 * @param mipmaps mipmap数量
 * @param usageType 是否用于帧缓冲区附加/采样
 */
void VK_CreateImage(VKImage_t *res, uint32_t width, uint32_t height, int format, bool cubeMap, int layers, int mipLevels, int usageType)
{
    VKPTRZERO(res);

    VkExtent3D extent3D = {
		width, height, 1
    };
    VkFormat imageFormat = (VkFormat)format;
    int depth = 0;
    int stencil = 0;
    switch (imageFormat)
    {
        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D16_UNORM:
            depth = 16;
            break;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            depth = 24;
            stencil = 8;
            break;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            depth = 32;
            stencil = 8;
            break;
        case VK_FORMAT_D32_SFLOAT:
            depth = 32;
            break;
        default:
            break;
    }
    VkImageCreateFlags flags = cubeMap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : (layers > 1 ? VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT : 0);
    int arrayLayers = cubeMap ? 6 : layers;

    VkImageUsageFlags usage = 0;
    if (usageType & IU_SAMPLER)
        usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT; // 采样
    if (usageType & IU_ATTACHMENT)
    {
        if (depth > 1)
            usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; // 深度模版附加
        else
            usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // 颜色附加
    }

    // 创建图像
    VkImageCreateInfo imageCreateInfo;
    VKZERO(imageCreateInfo);
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.flags = flags;
    imageCreateInfo.format = imageFormat; // 图像数据格式
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D; // 图像类型
    imageCreateInfo.mipLevels = mipLevels; // mipmap层数
    imageCreateInfo.extent = extent3D; // 图像大小
    imageCreateInfo.arrayLayers = arrayLayers;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = usage; // 图像用途
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT; // 图像采样数
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkImage image;
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    VmaMemoryRequirements memReq;
	VKZERO(memReq);
	memReq.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    memReq.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VmaAllocation allocation;
    VkResult result = vmaCreateImage(vk_context.vmaAllocator, &imageCreateInfo, &memReq, &image, &allocation, NULL);
    HANDLE_VK_ERROR_EXIT(result, "vmaCreateImage");

    res->vmaAllocation = allocation;
#else
    VkResult result = vkCreateImage(vk_context.device, &imageCreateInfo, VKCONTEXT_ALLOCATOR, &image);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateImage");

    // 计算图像内存大小
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vk_context.device, image, &memoryRequirements);

    int memoryTypeIndex = VK_QueryPhysicalDeviceMemoryType(vk_context.physicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VkMemoryAllocateInfo memoryAllocateInfo;
    VKZERO(memoryAllocateInfo);
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size; // 分配内存大小
    memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

    // 分配图像显存
    VkDeviceMemory memory;
    vkAllocateMemory(vk_context.device, &memoryAllocateInfo, VKCONTEXT_ALLOCATOR, &memory);
    HANDLE_VK_ERROR_EXIT(result, "vkAllocateMemory");

    // 绑定显存到图像
    result = vkBindImageMemory(vk_context.device, image, memory, 0);
    HANDLE_VK_ERROR_EXIT(result, "vkBindImageMemory");

    res->memory = memory;
#endif

    res->image = image;
    res->format = imageCreateInfo.format;
    res->width = width;
    res->height = height;
    res->mipLevels = mipLevels;
    res->usage = usageType;
    res->layers = layers;
    res->cubeMap = cubeMap;
	res->layout = VK_IMAGE_LAYOUT_UNDEFINED;

    // 创建图像视图
    VkImageViewType imageViewType = cubeMap ? VK_IMAGE_VIEW_TYPE_CUBE : (layers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D);
	VkImageAspectFlags viewAspectFlags = depth > 0 ? (VK_IMAGE_ASPECT_DEPTH_BIT | (stencil > 0 && (usageType & IU_ATTACHMENT) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0)) : VK_IMAGE_ASPECT_COLOR_BIT;

    res->imageView = VK_CreateImageView(image, format, imageViewType, viewAspectFlags, mipLevels, arrayLayers);

    // 如果是深度+模版图像, 且需要采样, 则生成第二个模版图像视图
	if(depth > 0 && stencil > 0 && (usageType & IU_SAMPLER))
	{
		res->imageViewSecondary = VK_CreateImageView(image, format, imageViewType, VK_IMAGE_ASPECT_STENCIL_BIT, mipLevels, arrayLayers);
	}
}

/**
 * 将缓冲区写入图像
 * @param image
 * @param buffer
 */
void VK_WriteImageBuffer(VKImage_t *image, VKBuffer_t *buffer, int offset, int offsets[], int sizes[], VkCommandBuffer cmd)
{
    if (image->mipLevels > 1)
    {
        // 复制数据到图像显存
        VK_CopyBufferToImage(image, buffer, offset, image->mipLevels, sizes, offsets, cmd);
    }
    else
    {
        // 复制数据到图像显存
        VK_CopyBufferToImage(image, buffer, offset, 1, NULL, NULL, cmd);
    }
}

/**
 * 创建采样器
 * @param sampler
 * @param linear 是否线性采样(包括mipmap层)
 * @param repeatU S分量超限采样规则
 * @param repeatV T分量超限采样规则
 * @param repeatW U分量超限采样规则
 * @param depthCmp 采样后是否进行深度比较
 * @param anisotropy
 * @param lodMax
 */
void VK_CreateSampler(VkSampler *sampler, bool minLinear, bool magLinear, bool mipmapLinear, int repeatU, int repeatV, int repeatW, bool depthCmp, float anisotropy, float lodMax)
{
    VkFilter minFilter = minLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    VkFilter magFilter = magLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    VkSamplerMipmapMode mipmapMode = mipmapLinear && lodMax > 0.0f ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;

    VkSamplerCreateInfo samplerCreateInfo;
    VKZERO(samplerCreateInfo);
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.minFilter = minFilter; // 缩小过滤
    samplerCreateInfo.magFilter = magFilter; // 放大过滤
    samplerCreateInfo.addressModeU = (VkSamplerAddressMode)repeatU; // 超限纹理坐标S采样模式
    samplerCreateInfo.addressModeV = (VkSamplerAddressMode)repeatV; // 超限纹理坐标T采样模式
    samplerCreateInfo.addressModeW = (VkSamplerAddressMode)repeatW; // 超限纹理坐标U采样模式
    samplerCreateInfo.anisotropyEnable = anisotropy > 1.0f;
    samplerCreateInfo.maxAnisotropy = anisotropy;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // 边框颜色
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    samplerCreateInfo.compareEnable = depthCmp ? VK_TRUE : VK_FALSE; // 采样时启用比较
    samplerCreateInfo.compareOp = depthCmp ? VK_COMPARE_OP_LESS_OR_EQUAL : VK_COMPARE_OP_ALWAYS; // 采样时比较方法
    samplerCreateInfo.mipmapMode = mipmapMode; // mipmap层间的采样过滤
    samplerCreateInfo.mipLodBias = 0; // 细节层偏移
    samplerCreateInfo.minLod = 0.0f; // 最小的细节层
    samplerCreateInfo.maxLod = lodMax; // 最大的细节层

    VkResult result = vkCreateSampler(vk_context.device, &samplerCreateInfo, VKCONTEXT_ALLOCATOR, sampler);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateSampler");
}

/**
 * 重置当前帧的命令缓冲区
 */
void VK_ResetCommandBuffer()
{
    VkResult result;

    result = vkResetCommandBuffer(vk_context.currentFrameCommandBuffer, 0);
    HANDLE_VK_ERROR_EXIT(result, "vkResetCommandBuffer");
}

/**
 * 开始记录到当前帧的命令缓冲区
 */
void VK_BeginCommandBuffer()
{
    VkResult result;

    if (vk_context.inCommandRecording)
    {
        common->Error("VK_BeginCommandBuffer: Vulkan main command buffer is beginning");
        return;
    }

    VK_ResetCommandBuffer();

    VkCommandBufferBeginInfo commandBufferBeginInfo;
    VKZERO(commandBufferBeginInfo);
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = 0; // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    result = vkBeginCommandBuffer(vk_context.currentFrameCommandBuffer, &commandBufferBeginInfo);
    HANDLE_VK_ERROR_EXIT(result, "vkBeginCommandBuffer");

    vk_context.inCommandRecording = true;
}

/**
 * 提交当前帧的命令
 */
void VK_SubmitCommand()
{
    VkResult result;

    VkSemaphore *waitSemaphores = &vk_context.currentFrameImageAvailableSemaphore;
    VkSemaphore *signalSemaphores = &vk_context.currentFrameRenderFinishedSemaphore;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo;
    VKZERO(submitInfo);
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.pWaitSemaphores = waitSemaphores; // 等待交换链图像准备完成的信号量
    submitInfo.waitSemaphoreCount = 1; // 等待的信号量数量
    submitInfo.pSignalSemaphores = signalSemaphores; // 触发渲染命令提交完成的信号量
    submitInfo.signalSemaphoreCount = 1; // 触发的信号量数量
    submitInfo.commandBufferCount = 1; // 提交的命令缓冲区数量
    submitInfo.pCommandBuffers = &vk_context.currentFrameCommandBuffer; // 提交的命令缓冲区

    result = vkQueueSubmit(vk_context.graphicsQueue.queue, 1, &submitInfo, vk_context.currentFrameInFlightFence);
    HANDLE_VK_ERROR_EXIT(result, "vkQueueSubmit");
}

/**
 * 结束记录当前帧的命令缓冲区
 */
void VK_EndCommandBuffer()
{
    if (vk_context.inCommandRecording)
    {
        vk_context.inCommandRecording = false;

        VkResult result = vkEndCommandBuffer(vk_context.currentFrameCommandBuffer);
        HANDLE_VK_ERROR_EXIT(result, "vkEndCommandBuffer");

        VK_SubmitCommand();
    }
    else
    {
        common->Error("VK_EndCommandBuffer: Vulkan main command buffer is not beginning");
        return;
    }
}

/**
 * 开始主渲染通道
 */
void VK_BeginRenderPass()
{
    if (vk_context.inRenderPass)
    {
        common->Error("VK_BeginRenderPass: Vulkan main render pass is beginning");
        return;
    }

    VkClearValue clearValues[2];

    VkRenderPassBeginInfo renderPassBeginInfo;
    VKZERO(renderPassBeginInfo);
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    // first using don't care on beginning
	if(vk_context.renderPassStage == RS_DONT_CARE)
	{
	    VKZERO(clearValues);
	    clearValues[0].color.float32[0] = 1.0f;
	    clearValues[0].color.float32[1] = 1.0f;
	    clearValues[0].color.float32[2] = 1.0f;
	    clearValues[0].color.float32[3] = 1.0f;
	    clearValues[1].depthStencil.depth = 1.0f;
	    clearValues[1].depthStencil.stencil = 128;

		renderPassBeginInfo.renderPass = vk_context.renderPass;
		renderPassBeginInfo.framebuffer = vk_context.swapchain.framebuffers[vk_context.swapchain.imageIndex].framebuffer;
		renderPassBeginInfo.clearValueCount = COUNT_OF(clearValues);
		renderPassBeginInfo.pClearValues = clearValues;
	}
	else // otherelse using load on beginning
	{
		renderPassBeginInfo.renderPass = vk_context.loadRenderPass;
		renderPassBeginInfo.framebuffer = vk_context.swapchain.loadFramebuffers[vk_context.swapchain.imageIndex].framebuffer;
	}
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = vk_context.swapchain.width;
    renderPassBeginInfo.renderArea.extent.height = vk_context.swapchain.height;

	vkCmdBeginRenderPass(vk_context.currentFrameCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vk_context.inRenderPass = true;
	vk_context.renderPassStage++;
}

/**
 * 结束主渲染通道
 */
void VK_EndRenderPass()
{
    if (vk_context.inRenderPass)
    {
        vk_context.inRenderPass = false;

        vkCmdEndRenderPass(vk_context.currentFrameCommandBuffer);
    }
    else
    {
        common->Error("VK_EndRenderPass: Vulkan main render pass is not beginning");
        return;
    }
}

/**
 * 将交换链图像布局改为复制源
 */
static void VK_SwapchainImageToCopySrc()
{
	IDVK_CHECK_RENDERPASS_IDLE();
    assert(vk_context.currentSwapchainImage.layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = vk_context.currentSwapchainImage.image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    imageMemoryBarrier.oldLayout = vk_context.currentSwapchainImage.layout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
	vk_context.currentSwapchainImage.layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
}

/**
 * 将交换链图像布局改为呈现
 */
static void VK_SwapchainImageToPresent()
{
    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = vk_context.currentSwapchainImage.image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = 0;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
	vk_context.currentSwapchainImage.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}

/**
 * 将交换链图像布局改回帧缓冲区附件
 */
static void VK_SwapchainImageToAttachment()
{
    if (vk_context.currentSwapchainImage.layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        return;

	IDVK_CHECK_RENDERPASS_IDLE();
    assert(vk_context.currentSwapchainImage.layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR || vk_context.currentSwapchainImage.layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = vk_context.currentSwapchainImage.image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.oldLayout = vk_context.currentSwapchainImage.layout; // VK_IMAGE_LAYOUT_UNDEFINED; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
	vk_context.currentSwapchainImage.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

/**
 * 将图像布局转为复制目标布局
 */
void VK_ImageToCopyDst(VKImage_t *image)
{
	IDVK_CHECK_RENDERPASS_IDLE();
	assert(image->layout == VK_IMAGE_LAYOUT_UNDEFINED || image->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	if(image->layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		return;

    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = image->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ? VK_ACCESS_SHADER_READ_BIT : 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.oldLayout = image->layout; // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    vkCmdPipelineBarrier(cmd,
        image->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
	image->layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
}

/**
 * 将图像布局转为采样布局
 */
void VK_ImageTransferDstToSampler(VKImage_t *image)
{
	IDVK_CHECK_RENDERPASS_IDLE();
    assert(image->layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
	image->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

/**
 * 拷贝当前颜色帧缓冲区图像到目标图像
 */
void VK_CopyColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY)
{
    const bool inRenderPass = vk_context.inRenderPass;

    if (inRenderPass)
	    VK_EndRenderPass();

    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VK_ImageToCopyDst(image);
    VK_SwapchainImageToCopySrc();

    VkImageCopy imageCopy;
    VKZERO(imageCopy);

    VkImageSubresourceLayers srcSubresource;
    VKZERO(srcSubresource);
    srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    srcSubresource.mipLevel = 0;
    srcSubresource.baseArrayLayer = 0;
    srcSubresource.layerCount = 1;

    VkImageSubresourceLayers dstSubresource;
    VKZERO(dstSubresource);
    dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    dstSubresource.mipLevel = 0;
    dstSubresource.baseArrayLayer = 0;
    dstSubresource.layerCount = 1;

    imageCopy.srcOffset.x = x;
    imageCopy.srcOffset.y = y;
    imageCopy.srcOffset.z = 0;
    imageCopy.dstOffset.x = dstX;
    imageCopy.dstOffset.y = dstY;
    imageCopy.dstOffset.z = 0;
    imageCopy.extent.width = width;
    imageCopy.extent.height = height;
    imageCopy.extent.depth = 1;
    imageCopy.srcSubresource = srcSubresource;
    imageCopy.dstSubresource = dstSubresource;

    vkCmdCopyImage(cmd, vk_context.currentSwapchainImage.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

    VK_SwapchainImageToAttachment();

    VK_ImageTransferDstToSampler(image);

    if (inRenderPass)
        VK_BeginRenderPass();
}

static void VK_SwapchainImageToCopy2(VkCommandBuffer cmd)
{
	IDVK_CHECK_RENDERPASS_IDLE();
    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = vk_context.currentSwapchainImage.image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
    vk_context.currentSwapchainImage.layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
}
static void VK_SwapchainImageToAttachment2(VkCommandBuffer cmd)
{
	IDVK_CHECK_RENDERPASS_IDLE();
    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = vk_context.currentSwapchainImage.image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    imageMemoryBarrier.dstAccessMask = 0;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
    vk_context.currentSwapchainImage.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}
/**
 * 拷贝当前颜色帧缓冲区图像到缓冲区
 */
void VK_CopyColorAttachmentImageToBuffer(VKBuffer_t *buffer, int x, int y, int width, int height, int offset, int cols, int rows, VkCommandBuffer cmd)
{
    const bool inRenderPass = vk_context.inRenderPass;

    if (inRenderPass)
        VK_EndRenderPass();

    VK_SwapchainImageToCopy2(cmd);

    VkBufferImageCopy bufferImageCopy;
    VKZERO(bufferImageCopy);

    bufferImageCopy.bufferOffset = offset;
    bufferImageCopy.bufferRowLength = cols;
    bufferImageCopy.bufferImageHeight = rows;

    VkImageSubresourceLayers srcSubresource;
    VKZERO(srcSubresource);
    srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    srcSubresource.mipLevel = 0;
    srcSubresource.baseArrayLayer = 0;
    srcSubresource.layerCount = 1;

    bufferImageCopy.imageOffset.x = x;
    bufferImageCopy.imageOffset.y = y;
    bufferImageCopy.imageOffset.z = 0;
    bufferImageCopy.imageExtent.width = width;
    bufferImageCopy.imageExtent.height = height;
    bufferImageCopy.imageExtent.depth = 1;
    bufferImageCopy.imageSubresource = srcSubresource;

    vkCmdCopyImageToBuffer(cmd, vk_context.currentSwapchainImage.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer->buffer, 1, &bufferImageCopy);

    VK_SwapchainImageToAttachment2(cmd);

    if (inRenderPass)
        VK_BeginRenderPass();
}

void VK_BlitColorAttachmentImage(VKImage_t *image, int x, int y, int width, int height, int dstX, int dstY, int dstWidth, int dstHeight, bool flipY)
{
    const bool inRenderPass = vk_context.inRenderPass;

    if (inRenderPass)
	    VK_EndRenderPass();

    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VK_ImageToCopyDst(image);
    VK_SwapchainImageToCopySrc();

    VkImageBlit imageBlit;
    VKZERO(imageBlit);

    VkImageSubresourceLayers srcSubresource;
    VKZERO(srcSubresource);
    srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    srcSubresource.mipLevel = 0;
    srcSubresource.baseArrayLayer = 0;
    srcSubresource.layerCount = 1;

    VkImageSubresourceLayers dstSubresource;
    VKZERO(dstSubresource);
    dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    dstSubresource.mipLevel = 0;
    dstSubresource.baseArrayLayer = 0;
    dstSubresource.layerCount = 1;

    imageBlit.srcOffsets[0].x = x;
    imageBlit.srcOffsets[0].y = flipY ? (y + height) : y;
    imageBlit.srcOffsets[0].z = 0;
    imageBlit.srcOffsets[1].x = x + width;
    imageBlit.srcOffsets[1].y = flipY ? y : (y + height);
    imageBlit.srcOffsets[1].z = 1;

    imageBlit.dstOffsets[0].x = dstX;
    imageBlit.dstOffsets[0].y = dstY;
    imageBlit.dstOffsets[0].z = 0;
    imageBlit.dstOffsets[1].x = dstX + dstWidth;
    imageBlit.dstOffsets[1].y = dstY + dstHeight;
    imageBlit.dstOffsets[1].z = 1;
    imageBlit.srcSubresource = srcSubresource;
    imageBlit.dstSubresource = dstSubresource;

    vkCmdBlitImage(cmd, vk_context.currentSwapchainImage.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

    VK_SwapchainImageToAttachment();

    VK_ImageTransferDstToSampler(image);

    if (inRenderPass)
        VK_BeginRenderPass();
}

/**
 * 同步栅栏
 * @param fence 栅栏
 */
void VK_WaitFence(VkFence fence)
{
    VkResult result;

    result = vkWaitForFences(vk_context.device, 1, &fence, VK_TRUE, UINT64_MAX);
    HANDLE_VK_ERROR_EXIT(result, "vkWaitForFences");

    result = vkResetFences(vk_context.device, 1, &fence);
    HANDLE_VK_ERROR_EXIT(result, "vkResetFences");
}

/**
 * 同步当前CPU帧
 */
void VK_WaitFrameFence()
{
    VK_WaitFence(vk_context.currentFrameInFlightFence);
}

/**
 * 获取下一帧的交换链图像
 * @return 将要渲染的交换链图像索引
 */
int VK_WaitSwapchainImage()
{
    uint32_t imageIndex;
    VkResult result;

    VK_WaitFrameFence();

    result = vkAcquireNextImageKHR(vk_context.device, vk_context.swapchain.swapchain, UINT64_MAX,
        vk_context.currentFrameImageAvailableSemaphore, // 交换链图像准备完毕触发信号量
        VK_NULL_HANDLE, &imageIndex);
    //HANDLE_VK_ERROR_WARN(result, "vkAcquireNextImageKHR");
    bool recreateSwapchain = false;
    if (result == VK_SUCCESS)
    {
        vk_context.swapchain.imageIndex = imageIndex;
    }
    else if (result == VK_TIMEOUT)
    {
        imageIndex = -1;
        vk_context.swapchain.imageIndex = (vk_context.swapchain.imageIndex + 1) % vk_context.swapchain.numImages;
        //return -1;
        //VK_RecreateSwapchain();
    }
    else if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        VK_RecreateSwapchain();
        //vk_context.swapchain.imageIndex = (vk_context.swapchain.imageIndex + 1) % vk_context.swapchain.numImages;
        recreateSwapchain = true;
    }
    else
        HANDLE_VK_ERROR_EXIT(result, "vkAcquireNextImageKHR");

    if (recreateSwapchain)
    {
        result = vkAcquireNextImageKHR(vk_context.device, vk_context.swapchain.swapchain, INT64_MAX, vk_context.currentFrameImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        HANDLE_VK_ERROR_EXIT(result, "vkAcquireNextImageKHR");
        vk_context.swapchain.imageIndex = imageIndex;
    }

    return imageIndex;
}

/**
 * 重置当前帧的描述符池
 */
void VK_ResetDescriptorPool()
{
    VkResult result;
    VKDescriptorPool_t *pool;

    VKQUEUE_FOREACH(VKDescriptorPool_t, vk_context.currentDescriptorPool)
    {
        pool = &vk_context.currentDescriptorPool.list[i];

        result = vkResetDescriptorPool(vk_context.device, pool->descriptorPool, 0);
        HANDLE_VK_ERROR_EXIT(result, "vkResetDescriptorPool");
        VKZERO(pool->descriptorSets);
        pool->usedDescriptorSets = 0;
        pool->usedUniforms = 0;
        pool->usedSamplers = 0;
    }

    VKQUEUE_RESET(VKDescriptorPool_t, vk_context.currentDescriptorPool);
}

/**
 * 呈现当前帧
 */
void VK_Present()
{
    VkResult result;

    VkSemaphore *waitSemaphores = &vk_context.currentFrameRenderFinishedSemaphore;

    VkPresentInfoKHR presentInfo;
    VKZERO(presentInfo);
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1; // 等待的信号量数量
    presentInfo.pWaitSemaphores = waitSemaphores; // 等待渲染命令提交完成的信号量
    presentInfo.swapchainCount = 1; // 呈现的交换链数量
    presentInfo.pSwapchains = &vk_context.swapchain.swapchain; // 呈现的交换链
    presentInfo.pImageIndices = &vk_context.swapchain.imageIndex; // 交换链的图像索引, 使用vkAcquireNextImageKHR返回
    presentInfo.pResults = NULL;

    result = vkQueuePresentKHR(vk_context.graphicsQueue.queue, &presentInfo); // 使用图像队列呈现
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
		HANDLE_VK_ERROR_EXIT(result, "vkQueuePresentKHR");
    }
}

/**
 * 开始渲染当前帧
 * @return 当前帧索引(0 - NUM_INFLIGHT_FRAME-1)
 */
int VK_BeginFrame()
{
    // 记录进入渲染帧
	vk_context.inRenderFrame = true;
    // 获取当前帧渲染的交换链图像
    VK_WaitSwapchainImage();
    // 重置当前帧的描述符池
    VK_ResetDescriptorPool();
    // 重置并开始当前帧的命令缓冲区
    VK_BeginCommandBuffer();
    // 发送将交换链图像布局转为附件布局命令
    VK_SwapchainImageToAttachment();
    // 发送开始主渲染通道的命令
    //VK_BeginRenderPass();
    // 重置为使用初始渲染通道
    vk_context.renderPassStage = RS_DONT_CARE;

    return vk_context.frameIndex;
}

/**
 * 结束当前帧的渲染
 */
void VK_EndFrame()
{
    // 发送结束主渲染通道命令
    //VK_EndRenderPass();
    // 发送将交换链图像布局转为呈现布局命令
    VK_SwapchainImageToPresent();
    // 结束当前使用的命令缓冲区
    VK_EndCommandBuffer();
    // 呈现
    VK_Present();
    // 切换下个帧
    VK_SwapBuffers();
    // 记录离开渲染帧
	vk_context.inRenderFrame = false;
}

/**
 * 交换当前帧索引
 * @return 当前帧索引(0 - NUM_INFLIGHT_FRAME-1)
 */
int VK_SwapBuffers()
{
    vk_context.frameIndex = (vk_context.frameIndex + 1) % NUM_INFLIGHT_FRAME;
    return vk_context.frameIndex;
}

/**
 * 分配一个渲染管线布局
 */
VKHandle_t VK_AllocPipelineLayout(VKPipelineLayout_t **pipelineLayout)
{
    VKPTRLIST_ALLOC(VKPipelineLayout_t, vk_context.pipelineLayouts);

	if(pipelineLayout)
		*pipelineLayout = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个渲染管线
 */
VKHandle_t VK_AllocPipeline(VKPipeline_t **pipeline)
{
    VKPTRLIST_ALLOC(VKPipeline_t, vk_context.pipelines);

	if(pipeline)
		*pipeline = _vkc_value;
    return _vkc_index;
}

static bool IsDescriptorPoolCanCreateSet(VKDescriptorPool_t *descriptorPool, int numUniforms, int numSamplers)
{
    return (descriptorPool->usedDescriptorSets + 1 <= IDVK_MAX_DESCRIPTOR_POOL_SETS
        && descriptorPool->usedUniforms + numUniforms <= IDVK_DESCRIPTOR_POOL_SIZE_UBO
        && descriptorPool->usedSamplers + numSamplers <= IDVK_DESCRIPTOR_POOL_SIZE_SAMPLER
        );
}

static bool IsDescriptorPoolFull(VKDescriptorPool_t *descriptorPool, int numUniforms, int numSamplers)
{
    return (descriptorPool->usedDescriptorSets + 1 > IDVK_MAX_DESCRIPTOR_POOL_SETS
        || descriptorPool->usedUniforms + numUniforms > IDVK_DESCRIPTOR_POOL_SIZE_UBO
        || descriptorPool->usedSamplers + numSamplers > IDVK_DESCRIPTOR_POOL_SIZE_SAMPLER
        );
}

/**
 * 分配一个描述符集
 */
VKHandle_t VK_AllocDescriptorSet(VkDescriptorSet **set, VkDescriptorPool *pool, int numUniforms, int numSamplers)
{
    VKDescriptorPool_t *descriptorPool = &VKQUEUE_CURRENT(VKDescriptorPool_t, vk_context.currentDescriptorPool);
    if (IsDescriptorPoolFull(descriptorPool, numUniforms, numSamplers))
    {
        VKQUEUE_NEXT(VKDescriptorPool_t, vk_context.currentDescriptorPool);
        if (_vkc_isNew)
        {
            VK_CreateDescriptorPool(_vkc_value);
        }
        else
        {
            _vkc_value->usedDescriptorSets = 0;
            _vkc_value->usedUniforms = 0;
            _vkc_value->usedSamplers = 0;
            VKZERO(_vkc_value->descriptorSets);
        }
        descriptorPool = _vkc_value;
    }

	int index = descriptorPool->usedDescriptorSets;
    VkDescriptorSet *descriptorSet = &descriptorPool->descriptorSets[index];
    VKPTRZERO(descriptorSet);
	descriptorPool->usedDescriptorSets++;
    descriptorPool->usedUniforms += numUniforms;
    descriptorPool->usedSamplers += numSamplers;
	if(set)
	    *set = descriptorSet;
    if(pool)
        *pool = descriptorPool->descriptorPool;
    return vk_context.currentDescriptorPool.used * IDVK_MAX_DESCRIPTOR_POOL_SETS + index;
}

/**
 * 分配一个着色器
 */
VKHandle_t VK_AllocShader(VkShaderModule **shaderModule)
{
    VKLIST_ALLOC(VkShaderModule, vk_context.shaders);
	if(shaderModule)
		*shaderModule = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个图像采样器
 */
VKHandle_t VK_AllocSampler(VkSampler **sampler)
{
    VKLIST_ALLOC(VkSampler, vk_context.samplers);
	if(sampler)
		*sampler = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个图像对象
 */
VKHandle_t VK_AllocImage(VKImage_t **image)
{
    VKPOOL_ALLOC(VKImage_t, vk_context.images);

	if(image)
		*image = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个缓冲区对象
 */
VKHandle_t VK_AllocBuffer(VKBuffer_t **buffer)
{
    VKPOOL_ALLOC(VKBuffer_t, vk_context.buffers);

	if(buffer)
		*buffer = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个栅栏
 */
VKHandle_t VK_AllocFence(VkFence **fence)
{
    VKLIST_ALLOC(VkFence, vk_context.fences);

	if(fence)
		*fence = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个命令池
 */
VKHandle_t VK_AllocCommandPool(VKCommandPool_t **commandPool)
{
    VKPTRLIST_ALLOC(VKCommandPool_t, vk_context.commandPools);

	if(commandPool)
		*commandPool = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个渲染通道
 */
VKHandle_t VK_AllocRenderPass(VkRenderPass **renderPass)
{
    VKLIST_ALLOC(VkRenderPass, vk_context.renderPasses);

	if(renderPass)
		*renderPass = _vkc_value;
    return _vkc_index;
}

/**
 * 分配一个查询池
 */
VKHandle_t VK_AllocQueryPool(VKQueryPool_t **queryPool)
{
    VKPOOL_ALLOC(VKQueryPool_t, vk_context.querys);

	if(queryPool)
		*queryPool = _vkc_value;
    return _vkc_index;
}

/**
 * 设置对象调试名称
 * @param objType 对象类型
 * @param handle 对象句柄
 * @param name 名称
 */
void VK_SetObjectName(int objType, uint64_t handle, const char* name)
{
    if (!vkSetDebugUtilsObjectNameEXT)
        return;

    VkDebugUtilsObjectNameInfoEXT createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    createInfo.objectType = (VkObjectType)objType;
    createInfo.objectHandle = handle;
    createInfo.pObjectName = name;
    vkSetDebugUtilsObjectNameEXT(vk_context.device, &createInfo);

    if (r_vkSetDebugInfo.GetInteger() > 1)
        Sys_Printf("Vulkan object %s %s: 0x%llx\n", VK_VkObjectTypeToName((VkObjectType)objType), name, handle);
}

void VK_DebugObjects()
{
#define SET_DEBUG_NAME(type, obj, name) VK_SetObjectName(VK_OBJECT_TYPE_##type, (uint64_t)vk_context.obj, name)
#define SET_DEBUG_VNAME(type, obj, ...) idStr::snPrintf(buffer, sizeof(buffer), __VA_ARGS__); VK_SetObjectName(VK_OBJECT_TYPE_##type, (uint64_t)vk_context.obj, buffer)
    char buffer[256];

    SET_DEBUG_NAME(INSTANCE, instance, "main::instance");
    SET_DEBUG_NAME(PHYSICAL_DEVICE, physicalDevice, "main::physicalDevice");
    SET_DEBUG_NAME(DEVICE, device, "main::device");
    SET_DEBUG_NAME(QUEUE, transferQueue.queue, "main::transferQueue");
    //SET_DEBUG_NAME(QUEUE, presentQueue.queue, "main::presentQueue");
    SET_DEBUG_NAME(QUEUE, graphicsQueue.queue, "main::graphicsQueue");
    SET_DEBUG_NAME(RENDER_PASS, renderPass, "main::renderPass");
    SET_DEBUG_NAME(COMMAND_POOL, commandPool.commandPool, "main::commandPool");
    SET_DEBUG_NAME(SWAPCHAIN_KHR, swapchain.swapchain, "main::swapchain");
    for (int i = 0; i < vk_context.swapchain.numImages; i++)
    {
        SET_DEBUG_VNAME(FRAMEBUFFER, swapchain.framebuffers[i].framebuffer, "main::swapchain::framebuffer%d", i);
        SET_DEBUG_VNAME(FRAMEBUFFER, swapchain.loadFramebuffers[i].framebuffer, "main::swapchain::loadFramebuffer%d", i);
        SET_DEBUG_VNAME(IMAGE, swapchain.framebuffers[i].image.image, "main::swapchain::image%d", i);
        SET_DEBUG_VNAME(IMAGE_VIEW, swapchain.framebuffers[i].image.imageView, "main::swapchain::imageView%d", i);
        SET_DEBUG_VNAME(IMAGE, swapchain.framebuffers[i].depthStencilImage.image, "main::swapchain::depthStencilImageView%d", i);
        SET_DEBUG_VNAME(IMAGE_VIEW, swapchain.framebuffers[i].depthStencilImage.imageView, "main::swapchain::depthStencilImage%d", i);
    }
    for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
    {
        SET_DEBUG_VNAME(COMMAND_BUFFER, commandPool.commandBuffers[i], "main::commandBuffer%d", i);
        SET_DEBUG_VNAME(FENCE, inFlightFences[i], "main::fence%d", i);
        SET_DEBUG_VNAME(SEMAPHORE, imageAvailableSemaphores[i], "main::imageAvailableSemaphore%d", i);
        SET_DEBUG_VNAME(SEMAPHORE, renderFinishedSemaphores[i], "main::renderFinishedSemaphore%d", i);
    }
}

bool VK_IsExtensionSupported(const char *name)
{
    for (int i = 0; i < vk_context.numExtensions; i++)
    {
        if (strcmp(name, vk_context.extensions[i]) == 0)
            return true;
    }
    return false;
}

/**
 * 创建查询池对象
 * @param query 返回查询池对象
 * @param queryType 查询池类型
 * @param num 每帧的数量
 */
void VK_CreateQueryPool(VKQueryPool_t *query, int queryType, int num)
{
	assert(num > 0);

	VkQueryPoolCreateInfo createInfo;
	VKZERO(createInfo);
	createInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	createInfo.queryType = (VkQueryType)queryType;
	createInfo.queryCount = num;

	VkQueryPool queryPool;
	VkResult result = vkCreateQueryPool(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &queryPool);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateQueryPool");

	query->queryPool = queryPool;
	query->numQuery = num;
	query->usedQuery = 0;

	//vkResetQueryPool(vk_context.device, query->queryPool, 0, query->numQuery * NUM_INFLIGHT_FRAME);
}

/**
 * 在当前命令缓冲区设置视口
 */
void VK_Viewport(int x, int y, int width, int height, float minZ, float maxZ)
{
    VkViewport viewport;
    VKZERO(viewport);
    viewport.x = (float)x;
    viewport.y = (float)y;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = minZ;
    viewport.maxDepth = maxZ;
    vkCmdSetViewport(vk_context.currentFrameCommandBuffer, 0, 1, &viewport);
}

/**
 * 在当前命令缓冲区设置裁剪区
 */
void VK_Scissor(int x, int y, int width, int height)
{
    VkRect2D scissor;
    VKZERO(scissor);
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = height;
    vkCmdSetScissor(vk_context.currentFrameCommandBuffer, 0, 1, &scissor);
}

/**
 * 在当前命令缓冲区设置线宽度
 */
void VK_LineWidth(float lineWidth)
{
    vkCmdSetLineWidth(vk_context.currentFrameCommandBuffer, lineWidth);
}

/**
 * 在当前命令缓冲区设置深度边界
 */
void VK_DepthBounds(float minz, float maxz)
{
    vkCmdSetDepthBounds(vk_context.currentFrameCommandBuffer, minz, maxz);
}

/**
 * 在当前命令缓冲区设置深度偏移
 */
void VK_PolygonOffset(float factor, float units)
{
    vkCmdSetDepthBias(vk_context.currentFrameCommandBuffer, factor, 0.0f, units);
}

/**
 * 在当前命令缓冲区设置面的模版掩码
 * @param face 面向
 * @param mask 掩码
 */
void VK_StencilMaskSeparate(int face, unsigned int mask)
{
    vkCmdSetStencilWriteMask(vk_context.currentFrameCommandBuffer, (VkStencilFaceFlags)face, mask);
}

/**
 * 在当前命令缓冲区设置模版两面的掩码
 * @param mask 掩码
 */
void VK_StencilMask(unsigned int mask)
{
    vkCmdSetStencilWriteMask(vk_context.currentFrameCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, mask);
}

/**
 * 在当前命令缓冲区设置模版面的比较参考值
 * @param face 面向
 * @param ref 比较参考值
 * @param mask 比较掩码
 */
void VK_StencilFuncSeparate(int face, int ref, int mask)
{
    vkCmdSetStencilReference(vk_context.currentFrameCommandBuffer, (VkStencilFaceFlags)face, ref);
    vkCmdSetStencilCompareMask(vk_context.currentFrameCommandBuffer, (VkStencilFaceFlags)face, mask);
}

/**
 * 在当前命令缓冲区设置模版两面的比较参考值
 * @param ref 比较参考值
 * @param mask 比较掩码
 */
void VK_StencilFunc(int ref, int mask)
{
    vkCmdSetStencilReference(vk_context.currentFrameCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, ref);
    vkCmdSetStencilCompareMask(vk_context.currentFrameCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, mask);
}

/**
 * 开始查询
 * @param query 查询池对象
 * @param offset 起始索引
 * @param precise 是否计数
 */
int VK_BeginQuery(VKQueryPool_t *query, int offset, bool precise)
{
    const bool useNew = offset == -1;

    if (useNew)
        offset = query->usedQuery;
    if (offset >= query->numQuery)
        return -1;

	VkQueryControlFlags flags = precise ? VK_QUERY_CONTROL_PRECISE_BIT : 0;
    vkCmdBeginQuery(vk_context.currentFrameCommandBuffer, query->queryPool, offset, flags);

    if (useNew)
        query->numQuery++;

    return offset;
}

/**
 * 结束查询
 * @param query 查询池对象
 * @param offset 起始索引
 */
void VK_EndQuery(VKQueryPool_t *query, int offset)
{
    if (offset == -1)
        offset = query->usedQuery;
    if (offset >= query->numQuery)
        return;

	vkCmdEndQuery(vk_context.currentFrameCommandBuffer, query->queryPool, offset);
}

/**
 * 获取查询结果(必须等待CPU同步后执行)
 * @param query 查询池对象
 * @param offset 起始索引
 * @param num 查询数量
 * @param ret 返回数据(总是uint64_t)
 */
void VK_GetQueryResult(VKQueryPool_t *query, int offset, int num, uint64_t *ret)
{
	if(num == 0)
		return;
	
	vkGetQueryPoolResults(vk_context.device, query->queryPool, offset, num, num * sizeof(uint64_t), ret, sizeof(uint64_t), VK_QUERY_RESULT_64_BIT /*| VK_QUERY_RESULT_WITH_AVAILABILITY_BIT*/ | VK_QUERY_RESULT_WAIT_BIT);
}

/**
 * 拷贝查询结果到缓冲区(必须等待CPU同步后执行)
 * @param query 查询池对象
 * @param offset 起始索引
 * @param num 查询数量
 * @param ret 返回数据(总是uint64_t)
 */
void VK_CopyQueryResult(VKQueryPool_t *query, int offset, int num, VKBuffer_t *buffer, int bufferOffset)
{
    if(num == 0)
        return;

    vkCmdCopyQueryPoolResults(vk_context.currentFrameCommandBuffer, query->queryPool, offset, num, buffer->buffer, bufferOffset, sizeof(uint64_t), VK_QUERY_RESULT_64_BIT /*| VK_QUERY_RESULT_WITH_AVAILABILITY_BIT*/ | VK_QUERY_RESULT_WAIT_BIT);
}

/**
 * 在当前命令缓冲区绘制数组
 * @param first 顶点偏移
 * @param count 顶点数量
 */
void VK_DrawArrays(int first, int count)
{
    vkCmdDraw(vk_context.currentFrameCommandBuffer, count, 1, first, 0);
}

/**
 * 在当前命令缓冲区按索引绘制
 * @param count 索引数量
 */
void VK_DrawElements(int count)
{
    vkCmdDrawIndexed(vk_context.currentFrameCommandBuffer, count, 1, 0, 0, 0);
}

/**
 * 在当前命令缓冲区按索引绘制
 * @param buffer
 * @param count
 * @param type 索引元素类型
 * @param offset
 */
void VK_DrawElementsEXT(const VKBuffer_t *buffer, int count, int type, int offset)
{
    vkCmdBindIndexBuffer(vk_context.currentFrameCommandBuffer, buffer->buffer, offset, (VkIndexType)type);
    vkCmdDrawIndexed(vk_context.currentFrameCommandBuffer, count, 1, 0, 0, 0);
}

void VK_SwapBufferHandle(VKHandle_t a, VKHandle_t b)
{
    VKPTRSWAP(VKBuffer_t, vk_context.buffers.list[a], vk_context.buffers.list[b])
}

void VK_SwapImageHandle(VKHandle_t a, VKHandle_t b)
{
    VKPTRSWAP(VKImage_t, vk_context.images.list[a], vk_context.images.list[b])
}

/**
 * 清空当前渲染通道的帧缓冲区的颜色附件
 */
void VK_ClearColor(float r, float g, float b, float a)
{
    VkClearAttachment clearAttachment;
    VKZERO(clearAttachment);

    clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clearAttachment.colorAttachment = 0;
    clearAttachment.clearValue.color.float32[0] = r;
    clearAttachment.clearValue.color.float32[1] = g;
    clearAttachment.clearValue.color.float32[2] = b;
    clearAttachment.clearValue.color.float32[3] = a;

    VkClearRect clearRect;
    VKZERO(clearRect);
    clearRect.rect.offset.x = 0;
    clearRect.rect.offset.y = 0;
    clearRect.rect.extent.width = vk_context.swapchain.width;
    clearRect.rect.extent.height = vk_context.swapchain.height;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;

    vkCmdClearAttachments(vk_context.currentFrameCommandBuffer, 1, &clearAttachment, 1, &clearRect);
}

/**
 * 清空当前渲染通道的帧缓冲区的深度附件
 */
void VK_ClearDepth(float depth)
{
    VkClearAttachment clearAttachment;
    VKZERO(clearAttachment);

    clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    clearAttachment.clearValue.depthStencil.depth = depth;

    VkClearRect clearRect;
    VKZERO(clearRect);
    clearRect.rect.offset.x = 0;
    clearRect.rect.offset.y = 0;
    clearRect.rect.extent.width = vk_context.swapchain.width;
    clearRect.rect.extent.height = vk_context.swapchain.height;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;

    vkCmdClearAttachments(vk_context.currentFrameCommandBuffer, 1, &clearAttachment, 1, &clearRect);
}

/**
 * 清空当前渲染通道的帧缓冲区的模版附件
 */
void VK_ClearStencil(int stencil)
{
    VkClearAttachment clearAttachment;
    VKZERO(clearAttachment);

    clearAttachment.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    clearAttachment.clearValue.depthStencil.depth = stencil;

    VkClearRect clearRect;
    VKZERO(clearRect);
    clearRect.rect.offset.x = 0;
    clearRect.rect.offset.y = 0;
    clearRect.rect.extent.width = vk_context.swapchain.width;
    clearRect.rect.extent.height = vk_context.swapchain.height;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;

    vkCmdClearAttachments(vk_context.currentFrameCommandBuffer, 1, &clearAttachment, 1, &clearRect);
}

/**
 * 清空当前渲染通道的帧缓冲区的深度和模版附件
 */
void VK_ClearDepthStencil(float depth, int stencil)
{
    VkClearAttachment clearAttachment;
    VKZERO(clearAttachment);

    clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    clearAttachment.clearValue.depthStencil.depth = depth;
    clearAttachment.clearValue.depthStencil.stencil = stencil;

    VkClearRect clearRect;
    VKZERO(clearRect);
    clearRect.rect.offset.x = 0;
    clearRect.rect.offset.y = 0;
    clearRect.rect.extent.width = vk_context.swapchain.width;
    clearRect.rect.extent.height = vk_context.swapchain.height;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;

    vkCmdClearAttachments(vk_context.currentFrameCommandBuffer, 1, &clearAttachment, 1, &clearRect);
}

/**
 * 清空当前渲染通道的帧缓冲区的附件
 */
void VK_Clear(int mask, float r, float g, float b, float a, float depth, int stencil)
{
    VkClearAttachment clearAttachments[2];
    VKZERO(clearAttachments);
    int numClearAttachments = 0;

    if (mask & VK_IMAGE_ASPECT_COLOR_BIT)
    {
        clearAttachments[numClearAttachments].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clearAttachments[numClearAttachments].colorAttachment = 0;
        clearAttachments[numClearAttachments].clearValue.color.float32[0] = r;
        clearAttachments[numClearAttachments].clearValue.color.float32[1] = g;
        clearAttachments[numClearAttachments].clearValue.color.float32[2] = b;
        clearAttachments[numClearAttachments].clearValue.color.float32[3] = a;
        numClearAttachments++;
    }

    int depthStencilMask = mask & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
    if (depthStencilMask)
    {
        if (depthStencilMask & VK_IMAGE_ASPECT_DEPTH_BIT)
        {
            clearAttachments[numClearAttachments].aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            clearAttachments[numClearAttachments].clearValue.depthStencil.depth = depth;
        }
        if (depthStencilMask & VK_IMAGE_ASPECT_STENCIL_BIT)
        {
            clearAttachments[numClearAttachments].aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            clearAttachments[numClearAttachments].clearValue.depthStencil.stencil = stencil;
        }
        numClearAttachments++;
    }

    if (numClearAttachments == 0)
        return;

    VkClearRect clearRect;
    VKZERO(clearRect);
    clearRect.rect.offset.x = 0;
    clearRect.rect.offset.y = 0;
    clearRect.rect.extent.width = vk_context.swapchain.width;
    clearRect.rect.extent.height = vk_context.swapchain.height;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;

    vkCmdClearAttachments(vk_context.currentFrameCommandBuffer, numClearAttachments, clearAttachments, 1, &clearRect);
}

/**
 * 清空当前帧的交换链缓冲区附件
 */
void VK_ClearSwapchainAttachments()
{
    VK_Clear(VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 128);
}

/**
 * 重置查询池(必须在命令记录时执行, 并且渲染通道未开始)
 * @param query 查询池
 */
void VK_ResetQueryPool(VKQueryPool_t *query)
{
	vkCmdResetQueryPool(vk_context.currentFrameCommandBuffer, query->queryPool, 0, query->numQuery);
    query->usedQuery = 0;
}

void VK_CreatePipelineCache(const void *data, int size)
{
	VkPipelineCacheCreateInfo createInfo;
	VKZERO(createInfo);
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	createInfo.initialDataSize = size;
	createInfo.pInitialData = data;

	VkResult result = vkCreatePipelineCache(vk_context.device, &createInfo, VKCONTEXT_ALLOCATOR, &vk_context.pipelineCache);
    HANDLE_VK_ERROR_RETURN(result, "vkCreatePipelineCache");
}

int VK_GetPipelineCacheData(void **data)
{
	if(VK_NULL_HANDLE == vk_context.pipelineCache)
		return -1;

	size_t size = 0;
	vkGetPipelineCacheData(vk_context.device, vk_context.pipelineCache, &size, NULL);
	
	if(size == 0)
		return 0;

	void *ptr = malloc(size);
	vkGetPipelineCacheData(vk_context.device, vk_context.pipelineCache, &size, ptr);
	*data = ptr;

	return size;
}

/**
 * 返回Vulkan的内存分配器
 */
VkAllocationCallbacks * VK_GetAllocator()
{
    return VKCONTEXT_ALLOCATOR;
}

/**
 * 销毁全局全部采样器
 */
static void VK_DestroySamplers()
{
    Sys_Printf("Destroy samplers: %d/%d\n", vk_context.samplers.num, vk_context.samplers.size);
    VKLIST_FOREACH(VkSampler, vk_context.samplers)
    {
        vkDestroySampler(vk_context.device, vk_context.samplers.list[i], VKCONTEXT_ALLOCATOR);
    }

    VKLIST_FREE(VkSampler, vk_context.samplers);
}

static void VK_DestroyPipelineCache()
{
	if(VK_NULL_HANDLE != vk_context.pipelineCache)
		vkDestroyPipelineCache(vk_context.device, vk_context.pipelineCache, VKCONTEXT_ALLOCATOR);
}

/**
 * 销毁全局全部栅栏
 */
static void VK_DestroyFences()
{
    Sys_Printf("Destroy fences: %d/%d\n", vk_context.fences.num, vk_context.fences.size);
    VKPTRLIST_FOREACH(VkFence, vk_context.fences)
    {
        vkDestroyFence(vk_context.device, vk_context.fences.list[i], VKCONTEXT_ALLOCATOR);
    }

    VKLIST_FREE(VkFence, vk_context.fences);
}

/**
 * 销毁全局全部渲染通道
 */
static void VK_DestroyRenderPasses()
{
    Sys_Printf("Destroy render passes: %d/%d\n", vk_context.renderPasses.num, vk_context.renderPasses.size);
    VKPTRLIST_FOREACH(VkRenderPass, vk_context.renderPasses)
    {
        vkDestroyRenderPass(vk_context.device, vk_context.renderPasses.list[i], VKCONTEXT_ALLOCATOR);
    }

    VKLIST_FREE(VkRenderPass, vk_context.renderPasses);
}

/**
 * 销毁命令池
 */
static void VK_DestroyCommandPool(VKCommandPool_t *commandPool)
{
    if (VK_NULL_HANDLE != commandPool->commandPool)
        vkDestroyCommandPool(vk_context.device, commandPool->commandPool, VKCONTEXT_ALLOCATOR);

    for (int i = 0; i < COUNT_OF(commandPool->fences); i++)
    {
        if (VK_NULL_HANDLE != commandPool->fences[i])
            vkDestroyFence(vk_context.device, commandPool->fences[i], VKCONTEXT_ALLOCATOR);
    }
}

/**
 * 销毁全局全部命令池
 */
static void VK_DestroyCommandPools()
{
    Sys_Printf("Destroy command pools: %d/%d\n", vk_context.commandPools.num, vk_context.commandPools.size);
    VKPTRLIST_FOREACH(VKCommandPool_t, vk_context.commandPools)
    {
        VK_DestroyCommandPool(vk_context.commandPools.list[i]);
    }

    VKPTRLIST_FREE(VKCommandPool_t, vk_context.commandPools);
}

/**
 * 销毁全局全部着色器
 */
static void VK_DestroyShaders()
{
    Sys_Printf("Destroy shaders: %d/%d\n", vk_context.shaders.num, vk_context.shaders.size);
    VKLIST_FOREACH(VkShaderModule, vk_context.shaders)
    {
        vkDestroyShaderModule(vk_context.device, vk_context.shaders.list[i], VKCONTEXT_ALLOCATOR);
    }

    VKLIST_FREE(VkShaderModule, vk_context.shaders);
}

/**
 * 销毁全局命令池和命令缓冲区
 */
static void VK_DestroyCommands()
{
    VK_DestroyCommandPool(&vk_context.commandPool);

    VKZERO(vk_context.commandPool);
}

/**
 * 销毁全局描述符池和描述符集
 */
static void VK_DestroyDescriptors()
{
	for(int m = 0; m < NUM_INFLIGHT_FRAME; m++)
	{
	    VKQUEUE_FOREACH(VKDescriptorPool_t, vk_context.descriptorPools[m])
	    {
	        vkDestroyDescriptorPool(vk_context.device, vk_context.descriptorPools[m].list[i].descriptorPool, VKCONTEXT_ALLOCATOR);
	    }

        VKQUEUE_FREE(VKDescriptorPool_t, vk_context.descriptorPools[m]);
	}

    VKZERO(vk_context.descriptorPools);
}

/**
 * 销毁全局信号量
 */
static void VK_DestroySemaphores()
{
	for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		if (VK_NULL_HANDLE != vk_context.imageAvailableSemaphores[i])
		{
			vkDestroySemaphore(vk_context.device, vk_context.imageAvailableSemaphores[i], VKCONTEXT_ALLOCATOR);
			vk_context.imageAvailableSemaphores[i] = VK_NULL_HANDLE;
		}
		if (VK_NULL_HANDLE != vk_context.renderFinishedSemaphores[i])
		{
			vkDestroySemaphore(vk_context.device, vk_context.renderFinishedSemaphores[i], VKCONTEXT_ALLOCATOR);
			vk_context.renderFinishedSemaphores[i] = VK_NULL_HANDLE;
		}
	}
}

/**
 * 销毁全局栅栏
 */
static void VK_DestroyInFlightFences()
{
    VkFence fence;

	for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
	{
		fence = vk_context.inFlightFences[i];
		if (VK_NULL_HANDLE != fence)
			vkDestroyFence(vk_context.device, fence, VKCONTEXT_ALLOCATOR);
	}
}

/**
 * 销毁全局逻辑设备
 */
static void VK_DestroyDevice()
{
    if (VK_NULL_HANDLE != vk_context.device)
    {
        vkDestroyDevice(vk_context.device, VKCONTEXT_ALLOCATOR);
        vk_context.device = VK_NULL_HANDLE;
    }
}

/**
 * 销毁图像(包括内存和视图)
 */
static void VK_DestroyImage(VKImage_t *image)
{
    if (VK_NULL_HANDLE != image->imageView)
        vkDestroyImageView(vk_context.device, image->imageView, VKCONTEXT_ALLOCATOR);
    if (VK_NULL_HANDLE != image->imageViewSecondary)
        vkDestroyImageView(vk_context.device, image->imageViewSecondary, VKCONTEXT_ALLOCATOR);
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
	/*
    if (VK_NULL_HANDLE != image->vmaAllocation)
        vmaFreeMemory(vk_context.vmaAllocator, image->vmaAllocation);
		*/
    if (VK_NULL_HANDLE != image->image)
        vmaDestroyImage(vk_context.vmaAllocator, image->image, image->vmaAllocation);
#else
    if (VK_NULL_HANDLE != image->memory)
        vkFreeMemory(vk_context.device, image->memory, VKCONTEXT_ALLOCATOR);
    if (VK_NULL_HANDLE != image->image)
        vkDestroyImage(vk_context.device, image->image, VKCONTEXT_ALLOCATOR);
#endif
}

/**
 * 销毁图像槽
 */
void VK_FreeImage(VKHandle_t handle)
{
    VKImage_t *image;

    if (handle == VKHANDLE_INVALID)
        return;

    assert(vk_context.images.list);

    image = vk_context.images.list[handle];

    if (image)
    {
        VK_DestroyImage(image);

        VKPOOL_FREESLOT(VKImage_t, vk_context.images, handle);
    }
}

/**
 * 获取图像槽索引
 */
VKHandle_t VK_FindImageHandle(VKImage_t *image)
{
    if (!image)
        return VKHANDLE_INVALID;

    VKPOOL_FOREACH(VKImage_t, vk_context.images)
    {
        if (image == vk_context.images.list[i])
        {
            return i;
        }
    }

    return VKHANDLE_INVALID;
}

/**
 * 销毁图像槽
 */
void VK_FreeImageObject(VKImage_t *image)
{
    if (!image)
        return;

    VKHandle_t handle = VK_FindImageHandle(image);
    if (handle != VKHANDLE_INVALID)
        VK_FreeImage(handle);
}

/**
 * 销毁全局图像表
 */
static void VK_DestroyImages()
{
    VKImage_t *image;

    Sys_Printf("Destroy images: %d/%d\n", vk_context.images.num, vk_context.images.size);
    VKPOOL_FOREACH(VKImage_t, vk_context.images)
    {
        image = vk_context.images.list[i];
        if (!image)
            continue;

        VK_DestroyImage(image);
    }

    VKPOOL_FREE(VKImage_t, vk_context.images);
}

/**
 * 销毁缓冲区(包括内存)
 */
void VK_DestroyBuffer(VKBuffer_t *buffer)
{
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    if (VK_NULL_HANDLE != buffer->buffer)
        vmaDestroyBuffer(vk_context.vmaAllocator, buffer->buffer, buffer->vmaAllocation);
	/*
    if (VK_NULL_HANDLE != buffer->vmaAllocation)
        vmaFreeMemory(vk_context.vmaAllocator, buffer->vmaAllocation);
		*/
#else
    if (VK_NULL_HANDLE != buffer->buffer)
        vkDestroyBuffer(vk_context.device, buffer->buffer, VKCONTEXT_ALLOCATOR);
    if (VK_NULL_HANDLE != buffer->memory)
        vkFreeMemory(vk_context.device, buffer->memory, VKCONTEXT_ALLOCATOR);
#endif
}

/**
 * 销毁缓冲区槽
 */
void VK_FreeBuffer(VKHandle_t handle)
{
    VKBuffer_t *buffer;

    if (handle == VKHANDLE_INVALID)
        return;

    assert(vk_context.buffers.list);

    buffer = vk_context.buffers.list[handle];

    if (buffer)
    {
        VK_DestroyBuffer(buffer);

        VKPOOL_FREESLOT(VKBuffer_t, vk_context.buffers, handle);
    }
}

/**
 * 获取缓冲区槽索引
 */
VKHandle_t VK_FindBufferHandle(VKBuffer_t *buffer)
{
    if (!buffer)
        return VKHANDLE_INVALID;

    VKPOOL_FOREACH(VKImage_t, vk_context.buffers)
    {
        if (buffer == vk_context.buffers.list[i])
        {
            return i;
        }
    }

    return VKHANDLE_INVALID;
}

/**
 * 销毁缓冲区槽
 */
void VK_FreeBufferObject(VKBuffer_t *buffer)
{
    if (!buffer)
        return;

    VKHandle_t handle = VK_FindBufferHandle(buffer);
    if (handle != VKHANDLE_INVALID)
        VK_FreeBuffer(handle);
}

/**
 * 销毁全局缓冲区表
 */
static void VK_DestroyBuffers()
{
    VKBuffer_t *buffer;

    Sys_Printf("Destroy buffers: %d/%d\n", vk_context.buffers.num, vk_context.buffers.size);
    VKPOOL_FOREACH(VKBuffer_t, vk_context.buffers)
    {
        buffer = vk_context.buffers.list[i];
        if (!buffer)
            continue;

        VK_DestroyBuffer(buffer);
    }

    VKPOOL_FREE(VKBuffer_t, vk_context.buffers);
}

/**
 */
void VK_DestroyQueryPool(VKQueryPool_t *query)
{
    if (VK_NULL_HANDLE != query->queryPool)
        vkDestroyQueryPool(vk_context.device, query->queryPool, VKCONTEXT_ALLOCATOR);
}

/**
 * 销毁全部查询池
 */
static void VK_DestroyQueryPools()
{
    VKQueryPool_t *query;

    Sys_Printf("Destroy query pools: %d/%d\n", vk_context.querys.num, vk_context.querys.size);
    VKPOOL_FOREACH(VKQueryPool_t, vk_context.querys)
    {
        query = vk_context.querys.list[i];
        if (!query)
            continue;

        VK_DestroyQueryPool(query);
    }

    VKPOOL_FREE(VKQueryPool_t, vk_context.querys);
}

/**
 * 销毁查询池
 */
void VK_FreeQueryPool(VKHandle_t handle)
{
    VKQueryPool_t *query;

    if (handle == VKHANDLE_INVALID)
        return;

    assert(vk_context.querys.list);

    query = vk_context.querys.list[handle];

    if (query)
    {
        VK_DestroyQueryPool(query);

        VKPOOL_FREESLOT(VKQueryPool_t, vk_context.querys, handle);
    }
}

/**
 * 销毁图层
 */
static void VK_DestroySurface()
{
    if (VK_NULL_HANDLE != vk_context.surface)
    {
        vkDestroySurfaceKHR(vk_context.instance, vk_context.surface, VKCONTEXT_ALLOCATOR);
        vk_context.surface = VK_NULL_HANDLE;
    }
}

/**
 * 销毁渲染通道
 */
static void VK_DestroyRenderPass()
{
    if (VK_NULL_HANDLE != vk_context.renderPass)
    {
        vkDestroyRenderPass(vk_context.device, vk_context.renderPass, VKCONTEXT_ALLOCATOR);
        vk_context.renderPass = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != vk_context.loadRenderPass)
    {
        vkDestroyRenderPass(vk_context.device, vk_context.loadRenderPass, VKCONTEXT_ALLOCATOR);
        vk_context.loadRenderPass = VK_NULL_HANDLE;
    }
}

/**
 * 销毁渲染管线布局
 */
static void VK_DestroyPipelineLayout(VKPipelineLayout_t *pipelineLayout)
{
    pipelineLayout->vertexShader = VK_NULL_HANDLE;
    pipelineLayout->fragmentShader = VK_NULL_HANDLE;
    if (VK_NULL_HANDLE != pipelineLayout->pipelineLayout)
        vkDestroyPipelineLayout(vk_context.device, pipelineLayout->pipelineLayout, VKCONTEXT_ALLOCATOR);
    if (VK_NULL_HANDLE != pipelineLayout->descriptorSetLayout)
        vkDestroyDescriptorSetLayout(vk_context.device, pipelineLayout->descriptorSetLayout, VKCONTEXT_ALLOCATOR);
}

/**
 * 销毁全局全部渲染管线布局
 */
static void VK_DestroyPipelineLayouts()
{
    Sys_Printf("Destroy pipeline layouts: %d/%d\n", vk_context.pipelineLayouts.num, vk_context.pipelineLayouts.size);

    VKPTRLIST_FOREACH(VKPipelineLayout_t, vk_context.pipelineLayouts)
    {
        VK_DestroyPipelineLayout(vk_context.pipelineLayouts.list[i]);
    }

    VKPTRLIST_FREE(VKPipelineLayout_t, vk_context.pipelineLayouts);
}

/**
 * 销毁渲染管线(包括顶点着色器模块/片段着色器模块/管线布局/描述符集布局)
 */
static void VK_DestroyPipeline(VKPipeline_t *pipeline)
{
    VKZERO(pipeline->pipelineLayout);

    if (VK_NULL_HANDLE != pipeline->pipeline)
        vkDestroyPipeline(vk_context.device, pipeline->pipeline, VKCONTEXT_ALLOCATOR);
}

/**
 * 销毁全局全部渲染管线
 */
static void VK_DestroyPipelines()
{
    Sys_Printf("Destroy pipelines: %d/%d\n", vk_context.pipelines.num, vk_context.pipelines.size);

    VKPTRLIST_FOREACH(VKPipeline_t, vk_context.pipelines)
    {
        VK_DestroyPipeline(vk_context.pipelines.list[i]);
    }

    VKPTRLIST_FREE(VKPipeline_t, vk_context.pipelines);
}

/**
 * 销毁交换链
 * @param keep 是否保留交换链对象
 */
static void VK_DestroySwapchain(bool keep)
{
    VKFramebuffer_t *fb;

    for (int i = 0; i < vk_context.swapchain.numImages; i++)
    {
        fb = &vk_context.swapchain.framebuffers[i];
        if (VK_NULL_HANDLE != fb->image.imageView)
            vkDestroyImageView(vk_context.device, fb->image.imageView, VKCONTEXT_ALLOCATOR);
        if (VK_NULL_HANDLE != fb->depthStencilImage.image)
		{
            VK_DestroyImage(&fb->depthStencilImage);
		}
        vkDestroyFramebuffer(vk_context.device, fb->framebuffer, VKCONTEXT_ALLOCATOR);
        vkDestroyFramebuffer(vk_context.device, vk_context.swapchain.loadFramebuffers[i].framebuffer, VKCONTEXT_ALLOCATOR);
    }

    if (VK_NULL_HANDLE != vk_context.swapchain.swapchain && !keep)
    {
        vkDestroySwapchainKHR(vk_context.device, vk_context.swapchain.swapchain, VKCONTEXT_ALLOCATOR);
        vk_context.swapchain.swapchain = VK_NULL_HANDLE;
    }

    VKARRAY_FREE2(VKFramebuffer_t, vk_context.swapchain.framebuffers, vk_context.swapchain.numImages);
}

/**
 * 销毁实例
 */
static void VK_DestroyInstance()
{
    if (VK_NULL_HANDLE != vk_context.instance)
    {
        vkDestroyInstance(vk_context.instance, VKCONTEXT_ALLOCATOR);
        vk_context.instance = VK_NULL_HANDLE;
    }
}

/**
 * 同步等待设备空闲
 */
void VK_WaitIdle()
{
    if(vk_context.device != VK_NULL_HANDLE)
        vkDeviceWaitIdle(vk_context.device);
}

/**
 * 重新创建交换链
 */
void VK_RecreateSwapchain()
{
    // 等待设备空闲
    VK_WaitIdle();
    // 销毁旧的交换链
    VK_DestroySwapchain(true);
    // 创建新的交换链
    VK_CreateSwapchain();
    // 创建交换链帧缓冲区
    VK_CreateSwapchainFramebuffers();
}

/**
 * 初始化Vulkan
 */
void VK_Init()
{
    if (vk_context.initialized)
        return;

    // 全局环境内存初始化
    VKZERO(vk_context);

    VKPOOL_INIT(VKImage_t, vk_context.images, IDVK_IMAGE_INIT_SIZE, IDVK_IMAGE_GRANULARITY);
    VKPOOL_INIT(VKBuffer_t, vk_context.buffers, IDVK_BUFFER_INIT_SIZE, IDVK_BUFFER_GRANULARITY);
    VKPTRLIST_INIT(VKPipeline_t, vk_context.pipelineLayouts, IDVK_PIPELINE_LAYOUT_INIT_SIZE, IDVK_PIPELINE_LAYOUT_GRANULARITY);
    VKPTRLIST_INIT(VKPipeline_t, vk_context.pipelines, IDVK_PILELINE_INIT_SIZE, IDVK_PIPELINE_GRANULARITY);
    VKLIST_INIT(VkSampler, vk_context.samplers, IDVK_SAMPLER_INIT_SIZE, IDVK_SAMPLER_GRANULARITY);
    VKLIST_INIT(VkShaderModule, vk_context.shaders, IDVK_SHADER_INIT_SIZE, IDVK_SHADER_GRANULARITY);
    VKLIST_INIT(VkFence, vk_context.fences, IDVK_FENCE_INIT_SIZE, IDVK_FENCE_GRANULARITY);
    VKPTRLIST_INIT(VKCommandPool_t, vk_context.commandPools, IDVK_COMMANDPOOL_INIT_SIZE, IDVK_COMMANDPOOL_GRANULARITY);
    VKLIST_INIT(VkRenderPass, vk_context.renderPasses, IDVK_RENDERPASS_INIT_SIZE, IDVK_RENDERPASS_GRANULARITY);
    VKPOOL_INIT(VKQueryPool_t, vk_context.querys, IDVK_QUERYPOOL_INIT_SIZE, IDVK_QUERYPOOL_GRANULARITY);
    for (int i = 0; i < NUM_INFLIGHT_FRAME; i++)
    {
        VKQUEUE_INIT(VKDescriptorPool_t, vk_context.descriptorPools[i], IDVK_DESCRIPTORPOOL_INIT_SIZE, IDVK_DESCRIPTORPOOL_GRANULARITY);
    }

    // 获取所有支持的验证层
    VK_QueryValidationLayers();
    // 获取所有支持的扩展
    VK_QueryExtensions();
    // 创建Vulkan实例
    VK_CreateInstance();
    // 获取所有物理设备
    VK_QueryPhysicalDevices();
    // 选择物理设备
    VK_ChoosePhysicalDevice();
    // 创建本地渲染图层
    VK_CreateSurface(nativeWindow);
    // 创建逻辑设备
    VK_CreateDevice();
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    // 创建VMA内存分配器
	VMA_CreatrAlloctor();
#endif
    // 创建交换链
    VK_CreateSwapchain();
    // 创建主渲染通道(渲染到交换链帧缓冲区)
    VK_CreateRenderPass();
    // 创建交换链帧缓冲区(需要渲染通道)
    VK_CreateSwapchainFramebuffers();
    // 创建同步信号量
    VK_CreateSemaphores();
    // 创建同步栅栏
    VK_CreateFences();
    // 创建命令池/命令缓冲区
    VK_CreateCommands();
    // 创建描述符池
    VK_CreateDescriptorPools();

    vk_context.initialized = true;
}

/**
 * 销毁Vulkan
 */
void VK_Destroy()
{
    if (!vk_context.initialized)
        return;

    // 等待设备空闲
    VK_WaitIdle();
    // 销毁采样器
    VK_DestroySamplers();
    // 销毁图像
    VK_DestroyImages();
    // 销毁缓冲区
    VK_DestroyBuffers();
    // 销毁描述符
    VK_DestroyDescriptors();
    // 销毁栅栏
    VK_DestroyFences();
    // 销毁渲染管线
    VK_DestroyRenderPasses();
    // 销毁命令池
    VK_DestroyCommandPools();
    // 销毁命令池/命令缓冲区
    VK_DestroyCommands();
    // 销毁渲染管线
    VK_DestroyPipelines();
    // 销毁渲染管线布局
    VK_DestroyPipelineLayouts();
    // 销毁着色器
    VK_DestroyShaders();
	// 销毁管线缓存
	VK_DestroyPipelineCache();
	// 销毁查询池
    VK_DestroyQueryPools();
    // 销毁交换链(完整)
    VK_DestroySwapchain(false);
    // 销毁渲染管线布局
    VK_DestroyRenderPass();
    // 销毁同步信号量
    VK_DestroySemaphores();
    // 销毁同步栅栏
    VK_DestroyInFlightFences();
    // 销毁渲染图层
    VK_DestroySurface();
#ifdef AMD_VULKAN_MEMORY_ALLOCATOR_H
    // 销毁VMA内存分配器
	VMA_DestroyAlloctor();
#endif
    // 销毁逻辑设备
    VK_DestroyDevice();
    // 销毁Vulkan实例
    VK_DestroyInstance();

    // 全局环境内存释放
    VKPTRARRAY_FREE2(char, vk_context.extensions, vk_context.numExtensions);
    VKPTRARRAY_FREE2(char, vk_context.validationLayers, vk_context.numValidationLayers);
    VKARRAY_FREE2(VkPhysicalDevice, vk_context.physicalDevices, vk_context.numPhysicalDevices);
    VKPTRARRAY_FREE2(char, vk_context.physicalExtensions, vk_context.numPhysicalExtensions);
    VKPTRARRAY_FREE2(char, vk_context.physicalValidationLayers, vk_context.numPhysicalValidationLayers);

    VKZERO(vk_context);

    vk_context.initialized = false;
}
