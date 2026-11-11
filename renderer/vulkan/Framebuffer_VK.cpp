#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

void idFramebuffer::Bind_VK()
{

}

void idFramebuffer::Unbind_VK()
{

}

void idFramebuffer::BindDirectly_VK()
{

}

void idFramebuffer::UnbindDirectly_VK()
{

}

void idFramebuffer::AttachImage2D_VK( int target, const idImage* image, int index )
{
    Create_VK(image, index);
}

void idFramebuffer::AttachImageDepth_VK( const idImage* image )
{
    Create_VK(image, 0);
}

void idFramebuffer::AttachImageDepthStencil_VK( const idImage* image )
{
    Create_VK(image, 0);
}

void idFramebuffer::AttachImageDepthLayer_VK( const idImage* image, int layer )
{
    Create_VK(image, layer);
}

void idFramebuffer::AttachImage2DLayer_VK( const idImage* image, int layer )
{
    Create_VK(image, layer);
}

void idFramebuffer::AttachImage2D_VK( const idImage* image )
{
    Create_VK(image, 0);
}

void idFramebuffer::AttachImageDepthSide_VK( const idImage* image, int side )
{
    Create_VK(image, side);
}

void idFramebuffer::AttachImage2DSide_VK( const idImage* image, int side )
{
    Create_VK(image, side);
}

void idFramebuffer::BeginRenderPass_VK()
{
    if (renderIndex < 0)
        return;

    if (vk_context.inCommandRecording)
    {
        VKOfflineRenderBuffer_t *renderBuffer = &vk_framebuffer[renderIndex];
        if (!renderBuffer->renderPassBegin)
        {
            VkClearValue clearValue;

            VkRenderPassBeginInfo renderPassBeginInfo;
            VKZERO(renderPassBeginInfo);
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            // first using don't care on beginning
            VKZERO(clearValue);
            if (renderBuffer->isDepth)
            {
                clearValue.depthStencil.depth = 1.0f;
                clearValue.depthStencil.stencil = 128;
            }
            else
            {
                clearValue.color.float32[0] = 1.0f;
                clearValue.color.float32[1] = 1.0f;
                clearValue.color.float32[2] = 1.0f;
                clearValue.color.float32[3] = 1.0f;
            }

            renderPassBeginInfo.renderPass = renderBuffer->renderPass;
            renderPassBeginInfo.framebuffer = renderBuffer->framebuffer->framebuffer;

            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearValue;
            renderPassBeginInfo.renderArea.offset.x = 0;
            renderPassBeginInfo.renderArea.offset.y = 0;
            renderPassBeginInfo.renderArea.extent.width = vk_context.swapchain.width;
            renderPassBeginInfo.renderArea.extent.height = vk_context.swapchain.height;

            vkCmdBeginRenderPass(vk_context.currentFrameCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            renderBuffer->renderPassBegin = true;
        }
    }
}

void idFramebuffer::EndRenderPass_VK()
{
    if (renderIndex < 0)
        return;

    if (vk_context.inCommandRecording)
    {
        VKOfflineRenderBuffer_t *renderBuffer = &vk_framebuffer[renderIndex];
        if (renderBuffer->renderPassBegin)
        {
            vkCmdEndRenderPass(vk_context.currentFrameCommandBuffer);

            renderBuffer->renderPassBegin = false;
        }
    }
}

void idFramebuffer::CreateRenderPass_VK( const idImage* image, int layer )
{
    VKImage_t *vkImg = (VKImage_t *)image->vk_image;
    bool isDepth = vkImg->format == VK_FORMAT_D32_SFLOAT || vkImg->format == VK_FORMAT_D24_UNORM_S8_UINT;

    VkAttachmentDescription attachment;
    VKZERO(attachment);
    //附件
    attachment.format = vkImg->format; // 颜色图像格式
    attachment.samples = VK_SAMPLE_COUNT_1_BIT; // 采样数
    attachment.initialLayout = vkImg->layout; // 图像附件初始布局
    if (isDepth)
    {
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 颜色附件加载时执行操作
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // 颜色附件存储时的操作
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // 模版附件加载时执行操作
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE; // 模版附件存储时执行操作
        attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // 图像附件最终布局
    }
    else
    {
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // 颜色附件加载时执行操作
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 颜色附件存储时的操作
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // 模版附件加载时执行操作
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // 模版附件存储时执行操作
        attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // 图像附件最终布局
    }

    // 附件
    VkAttachmentReference attachmentRef;
    VKZERO(attachmentRef);
    attachmentRef.attachment = 0; // 附件索引
    attachmentRef.layout = isDepth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // 子通道
    VkSubpassDescription subpassDescription;
    VKZERO(subpassDescription);
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    if (isDepth)
    {
        subpassDescription.colorAttachmentCount = 0; // 颜色附件数
        subpassDescription.pColorAttachments = NULL; // 颜色附件
        subpassDescription.pDepthStencilAttachment = &attachmentRef; // 深度模版附件
    }
    else
    {
        subpassDescription.colorAttachmentCount = 1; // 颜色附件数
        subpassDescription.pColorAttachments = &attachmentRef; // 颜色附件
        subpassDescription.pDepthStencilAttachment = NULL; // 深度模版附件
    }

    // 子通道阶段依赖
    VkSubpassDependency subpassDependency;
    VKZERO(subpassDependency);
    // 颜色通道
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL; // 当前从子通道外部进入
    subpassDependency.dstSubpass = 0; // 进入子通道0
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    subpassDependency.srcAccessMask = 0;
    if (isDepth)
    {
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    else
    {
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    VkRenderPassCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = 1; // 渲染通道附件数
    createInfo.pAttachments = &attachment; // 渲染通道附件
    createInfo.dependencyCount = 1; // 渲染子通道依赖数
    createInfo.pDependencies = &subpassDependency; // 渲染子通道依赖
    createInfo.subpassCount = 1; // 渲染子通道数量
    createInfo.pSubpasses = &subpassDescription; // 渲染子通道

    VkResult result = vkCreateRenderPass(vk_context.device, &createInfo, VK_GetAllocator(), &vk_framebuffer[layer].renderPass);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateRenderPass");
}

void idFramebuffer::CreateFramebuffer_VK( const idImage* image, int layer )
{
    VkResult result;

    VKImage_t *vkImg = (VKImage_t *)image->vk_image;
    bool isDepth = vkImg->format == VK_FORMAT_D32_SFLOAT || vkImg->format == VK_FORMAT_D24_UNORM_S8_UINT;

    if (VK_NULL_HANDLE == vk_framebuffer[layer].renderPass)
    {
        CreateRenderPass_VK(image, layer);
    }

    if (VK_NULL_HANDLE == vk_framebuffer[layer].imageView)
    {
        vk_framebuffer[layer].imageView = VK_CreateImageView(vkImg->image, vkImg->format, 0, 0, 1, 1);
    }

    vkImg->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkImageView attachment = vk_framebuffer[layer].imageView;

    VkFramebufferCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = vk_framebuffer[layer].renderPass; // 渲染通道
    createInfo.attachmentCount = 1; // 帧缓冲区附件数量
    createInfo.pAttachments = &attachment; // 帧缓冲区附件
    createInfo.width = width; // 宽度
    createInfo.height = height; // 高度
    createInfo.layers = 1;

    result = vkCreateFramebuffer(vk_context.device, &createInfo, VK_GetAllocator(), &vk_framebuffer[layer].framebuffer->framebuffer);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateFramebuffer");

    if (isDepth)
        vk_framebuffer[layer].framebuffer->depthStencilImage = *vkImg;
    else
        vk_framebuffer[layer].framebuffer->image = *vkImg;
}

void idFramebuffer::Create_VK( const idImage* image, int layer )
{
    assert(layer >= 0 && layer < COUNT_OF(vk_framebuffer));

    if (vk_framebuffer[layer].framebuffer)
        return;

    CreateFramebuffer_VK(image, layer);
}
