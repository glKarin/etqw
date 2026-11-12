#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef _VULKAN
#include "imgui/backends/imgui_impl_vulkan.h"
#include "renderer/vulkan/vk_local.h"

static void check_vk_result(VkResult err)
{
    HANDLE_VK_ERROR_EXIT(err, "imGui");
}

static void VKimp_ImGui_Init(void)
{
    // Setup Platform/Renderer backends
#if 1
    ImGui_ImplSDL2_InitForVulkan(window);
#else
    //ImGui_ImplAndroid_SetGeometry(screen_width, screen_height);
#endif

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vk_context.instance;
    init_info.PhysicalDevice = vk_context.physicalDevice;
    init_info.Device = vk_context.device;
    init_info.QueueFamily = vk_context.graphicsQueue.familyIndex;
    init_info.Queue = vk_context.graphicsQueue.queue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = VK_NULL_HANDLE;
    init_info.RenderPass = vk_context.renderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = vk_context.swapchain.numImages;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = NULL;
    init_info.CheckVkResultFn = check_vk_result;
    init_info.DescriptorPoolSize = 1024;
    ImGui_ImplVulkan_Init(&init_info);

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = true;
}
#endif

void GLimp_ImGui_Init(void)
{
#ifdef _VULKAN
    if (USING_VULKAN)
    {
        VKimp_ImGui_Init();
        return;
    }
#endif
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
#ifdef _OPENGLES3
	if(USING_GLES3)
		ImGui_ImplOpenGL3_Init("#version 300 es");
	else
#endif
	ImGui_ImplOpenGL3_Init("#version 100");

    ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true;
}

void GLimp_ImGui_NewFrame(void)
{
    // Start the Dear ImGui frame
#ifdef _VULKAN
    if (USING_VULKAN)
        ImGui_ImplVulkan_NewFrame();
    else
#endif
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
}

void GLimp_ImGui_EndFrame(void)
{
#ifdef _VULKAN
    if (USING_VULKAN)
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vk_context.currentFrameCommandBuffer);
    else
#endif
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLimp_ImGui_Shutdown(void)
{
    // Cleanup
#ifdef _VULKAN
    if (USING_VULKAN)
        ImGui_ImplVulkan_Shutdown();
    else
#endif
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplSDL2_Shutdown();
}


