#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

void R_VK_DepthStencilImageToAttachment(VKImage_t *image, int mask)
{
    assert(image->layout == VK_IMAGE_LAYOUT_UNDEFINED || image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    if(image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        return;

    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageAspectFlags aspectFlags = 0;
    if (mask & IA_DEPTH)
        aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if (mask & IA_STENCIL)
        aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = aspectFlags;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = image->layers;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ? VK_ACCESS_SHADER_READ_BIT : 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.oldLayout = image->layout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    vkCmdPipelineBarrier(cmd,
        image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
    image->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}

void R_VK_DepthStencilImageToSampler(VKImage_t *image, int mask)
{
    assert(image->layout == VK_IMAGE_LAYOUT_UNDEFINED || image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    if(image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL)
        return;

    VkCommandBuffer cmd = vk_context.currentFrameCommandBuffer;

    VkImageAspectFlags aspectFlags = 0;
    if (mask & IA_DEPTH)
        aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if (mask & IA_STENCIL)
        aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;

    VkImageSubresourceRange subresourceRange;
    VKZERO(subresourceRange);
    subresourceRange.aspectMask = aspectFlags;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = image->layers;

    VkImageMemoryBarrier imageMemoryBarrier;
    VKZERO(imageMemoryBarrier);
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image->image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    imageMemoryBarrier.oldLayout = image->layout;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    vkCmdPipelineBarrier(cmd,
        image->layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ? VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0,
                         NULL, 1, &imageMemoryBarrier);
    image->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
}

void idFramebuffer::Bind_VK()
{
	if (vk_context.inRenderFrame)
	{
	    if (vk_context.inCommandRecording)
	    {
	        if (vk_context.inRenderPass)
	        {
	            VK_EndRenderPass();
	            R_VK_DepthStencilImageToAttachment(vk_framebuffer.image, IA_DEPTH_STENCIL);
	        }
	    }
	}
}

void idFramebuffer::Unbind_VK()
{
	if (vk_context.inRenderFrame)
	{
	    if (vk_context.inCommandRecording)
	    {
	        EndRenderPass_VK();
	        vk_framebuffer.layer = -1;

	        R_VK_DepthStencilImageToSampler(vk_framebuffer.image, IA_DEPTH_STENCIL);

	        /*if (!vk_context.inRenderPass)
	        {
	            VK_BeginRenderPass();
	        }*/
	    }
	}
	R_VK_SetRenderPass(vk_context.renderPass, 0, 0);
}

void idFramebuffer::BindDirectly_VK()
{
     VK_EndRenderPass();
}

void idFramebuffer::UnbindDirectly_VK()
{
     EndRenderPass_VK();

     vk_framebuffer.layer = -1;

    R_VK_SetRenderPass(vk_context.renderPass, 0, 0);
     //VK_BeginRenderPass();
}

void idFramebuffer::AttachImage2D_VK( int target, const idImage* image, int index )
{
}

void idFramebuffer::AttachImageDepth_VK( const idImage* image )
{
}

void idFramebuffer::AttachImageDepthStencil_VK( const idImage* image )
{
}

void idFramebuffer::AttachImageDepthLayer_VK( const idImage* image, int layer )
{
    CreateFramebuffer_VK(image, layer);

    if (vk_context.inCommandRecording)
    {
        Bind_VK();

		EndRenderPass_VK();
        vk_framebuffer.stage = clear ? RS_DONT_CARE : RS_LOAD;
        vk_framebuffer.layer = layer;
        BeginRenderPass_VK();
    }
}

void idFramebuffer::AttachImage2DLayer_VK( const idImage* image, int layer )
{
}

void idFramebuffer::AttachImage2D_VK( const idImage* image )
{
}

void idFramebuffer::AttachImageDepthSide_VK( const idImage* image, int side )
{
}

void idFramebuffer::AttachImage2DSide_VK( const idImage* image, int side )
{
}

void idFramebuffer::Reset_VK( void )
{
	VKZERO(vk_framebuffer);
    vk_framebuffer.layer = -1;
    clear = false;
}

void idFramebuffer::SetClear(bool on)
{
    clear = on;
}

void idFramebuffer::BeginRenderPass_VK()
{
    if (vk_framebuffer.layer < 0 || !vk_framebuffer.framebuffers[vk_framebuffer.stage].framebuffer[vk_framebuffer.layer])
        return;

    if (vk_context.inCommandRecording)
    {
        if (!vk_framebuffer.renderPassBegin)
        {

            VKRenderFramebufferStage_t *stage = &vk_framebuffer.framebuffers[vk_framebuffer.stage];

            VkClearValue clearValue;

            VkRenderPassBeginInfo renderPassBeginInfo;
            VKZERO(renderPassBeginInfo);
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            VKZERO(clearValue);
            if (vk_framebuffer.image->type > IA_COLOR)
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

            renderPassBeginInfo.renderPass = stage->renderPass;
            renderPassBeginInfo.framebuffer = stage->framebuffer[vk_framebuffer.layer]->framebuffer;

            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearValue;
            renderPassBeginInfo.renderArea.offset.x = 0;
            renderPassBeginInfo.renderArea.offset.y = 0;
            renderPassBeginInfo.renderArea.extent.width = width;
            renderPassBeginInfo.renderArea.extent.height = height;

            vkCmdBeginRenderPass(vk_context.currentFrameCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			R_VK_SetRenderPass(stage->renderPass, width, height);
            vk_framebuffer.renderPassBegin = true;
        }
    }
}

void idFramebuffer::EndRenderPass_VK()
{
    if (vk_framebuffer.layer < 0 || !vk_framebuffer.framebuffers[vk_framebuffer.stage].framebuffer[vk_framebuffer.layer])
        return;

    if (vk_context.inCommandRecording)
    {
        if (vk_framebuffer.renderPassBegin)
        {
            vkCmdEndRenderPass(vk_context.currentFrameCommandBuffer);

            vk_framebuffer.renderPassBegin = false;
        }
    }
}

void idFramebuffer::CreateFramebuffer_VK( const idImage* image, int layer )
{
    VkResult result;

    assert(layer >= 0 && layer < COUNT_OF(vk_framebuffer.imageView));

    VKRenderFramebufferStage_t *stage = &vk_framebuffer.framebuffers[clear ? 0 : 1];

    if (stage->framebuffer[layer])
        return;

    VK_AllocFramebuffer(&stage->framebuffer[layer]);

    VKImage_t *vkImg = (VKImage_t *)image->vk_image;
    const bool isDepth = vkImg->type > IA_COLOR;

    if (VK_NULL_HANDLE == stage->renderPass)
    {
        stage->renderPass = R_VK_CreateShadowMappingRenderPass(!clear);
    }

    if (VK_NULL_HANDLE == vk_framebuffer.imageView[layer])
    {
        VkImageView *imageView = &vk_framebuffer.imageView[layer];
        VK_AllocImageView(&imageView);
        *imageView = VK_CreateImageView(vkImg->image, vkImg->format, VK_IMAGE_VIEW_TYPE_2D, isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_COLOR_BIT, 1, 1, layer);
        vk_framebuffer.imageView[layer] = *imageView;
        VKDEBUG_SET_NAME(VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)vk_framebuffer.imageView[layer], "framebuffer_%s::image_view_%d", fboName.c_str(), layer);
    }

    VkImageView attachment = vk_framebuffer.imageView[layer];

    VkFramebufferCreateInfo createInfo;
    VKZERO(createInfo);
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = stage->renderPass; // 渲染通道
    createInfo.attachmentCount = 1; // 帧缓冲区附件数量
    createInfo.pAttachments = &attachment; // 帧缓冲区附件
    createInfo.width = width; // 宽度
    createInfo.height = height; // 高度
    createInfo.layers = 1;

    result = vkCreateFramebuffer(vk_context.device, &createInfo, VK_GetAllocator(), &stage->framebuffer[layer]->framebuffer);
    HANDLE_VK_ERROR_EXIT(result, "vkCreateFramebuffer");
    VKDEBUG_SET_NAME(VK_OBJECT_TYPE_FRAMEBUFFER, (uint64_t)stage->framebuffer[layer]->framebuffer, "framebuffer_%s_%s::framebuffer_%d", fboName.c_str(), clear ? "clear" : "load", layer);

    if (isDepth)
        stage->framebuffer[layer]->depthStencilImage = *vkImg;
    else
        stage->framebuffer[layer]->image = *vkImg;

    vk_framebuffer.image = vkImg;
}
