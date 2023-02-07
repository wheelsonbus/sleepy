#if defined(ZZ__VULKAN)

#include "render_pass.h"

b8 internal_vulkan_render_pass_create(struct internal_vulkan_render_pass* render_pass, struct internal_vulkan_render_pass_config* config)
{
    render_pass->device = config->device;

    VkAttachmentDescription colorAttachmentDescription;
    colorAttachmentDescription.flags = 0;
    colorAttachmentDescription.format = render_pass->device->swapchain_support_details.surfaceFormat.format;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription attachmentDescriptions[1] = {colorAttachmentDescription};

    VkAttachmentReference colorAttatchmentReference;
    colorAttatchmentReference.attachment = 0;
    colorAttatchmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription;
    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = ZZ_NULL;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttatchmentReference;
    subpassDescription.pResolveAttachments = ZZ_NULL;
    subpassDescription.pDepthStencilAttachment = ZZ_NULL;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = ZZ_NULL;

    VkSubpassDescription subpassDescriptions[1] = {subpassDescription};

    VkSubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency subpassDependencies[1] = {subpassDependency};

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = ZZ_NULL;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = attachmentDescriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = subpassDescriptions;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = subpassDependencies;

    if (vkCreateRenderPass(render_pass->device->device, &renderPassCreateInfo, ZZ_NULL, &render_pass->renderPass) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    return ZZ_TRUE;
}

void internal_vulkan_render_pass_destroy(struct internal_vulkan_render_pass* render_pass)
{
    vkDestroyRenderPass(render_pass->device->device, render_pass->renderPass, ZZ_NULL);
    render_pass->renderPass = VK_NULL_HANDLE;
}

#endif