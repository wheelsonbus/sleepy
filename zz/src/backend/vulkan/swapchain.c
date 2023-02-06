#if defined(ZZ_BACKEND_VULKAN)

#include "swapchain.h"

b8 backend_vulkan_swapchain_create(struct backend_vulkan_swapchain* swapchain, const struct backend_vulkan_swapchain_config* config)
{
    swapchain->surface = config->surface;
    swapchain->device = config->device;
    swapchain->render_pass = config->render_pass;

    u16 width, height;
    application_get_size(&width, &height);
    if (width == 0 || height == 0)
    {
        swapchain->extent = swapchain->device->swapchain_support_details.surfaceCapabilities.currentExtent; // TODO Handle currentExtent.width/height == UINT32_MAX?
    }
    else
    {
        swapchain->extent.width = (uint32_t)width;
        swapchain->extent.height = (uint32_t)height;
    }

    uint32_t imageCount = swapchain->device->swapchain_support_details.surfaceCapabilities.minImageCount + 1;
    if (swapchain->device->swapchain_support_details.surfaceCapabilities.maxImageCount != 0 && imageCount > swapchain->device->swapchain_support_details.surfaceCapabilities.maxImageCount)
    {
        imageCount = swapchain->device->swapchain_support_details.surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = ZZ_NULL;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = swapchain->surface->surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = swapchain->device->swapchain_support_details.surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = swapchain->device->swapchain_support_details.surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = swapchain->extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    uint32_t queueFamilyIndices[2] = {swapchain->device->queue_family_indices.graphics, swapchain->device->queue_family_indices.present};
    if (swapchain->device->queue_family_indices.graphics == swapchain->device->queue_family_indices.present)
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = ZZ_NULL;
    }
    else
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    swapchainCreateInfo.preTransform = swapchain->device->swapchain_support_details.surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = swapchain->device->swapchain_support_details.presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(swapchain->device->device, &swapchainCreateInfo, ZZ_NULL, &swapchain->swapchain) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    vkGetSwapchainImagesKHR(swapchain->device->device, swapchain->swapchain, &imageCount, ZZ_NULL);
    memory_array_create_and_reserve(&swapchain->images, imageCount);
    vkGetSwapchainImagesKHR(swapchain->device->device, swapchain->swapchain, &imageCount, swapchain->images.data);
    swapchain->images.length = imageCount;

    memory_array_create_and_reserve(&swapchain->imageViews, imageCount);
    for (uint32_t i = 0; i < imageCount; i += 1)
    {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = ZZ_NULL;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = swapchain->images.data[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = swapchain->device->swapchain_support_details.surfaceFormat.format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(swapchain->device->device, &imageViewCreateInfo, ZZ_NULL, &swapchain->imageViews.data[i]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }
    }
    swapchain->imageViews.length = imageCount;

    memory_array_create_and_reserve(&swapchain->framebuffers, imageCount);
    for (uint32_t i = 0; i < imageCount; i += 1)
    {
        VkImageView attachmentImageViews[1] = {swapchain->imageViews.data[i]};

        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = ZZ_NULL;
        framebufferCreateInfo.flags = 0;
        framebufferCreateInfo.renderPass = swapchain->render_pass->renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = attachmentImageViews;
        framebufferCreateInfo.width = swapchain->extent.width;
        framebufferCreateInfo.height = swapchain->extent.height;
        framebufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(swapchain->device->device, &framebufferCreateInfo, ZZ_NULL, &swapchain->framebuffers.data[i]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }
    }
    swapchain->framebuffers.length = imageCount;

    return ZZ_TRUE;
}

void backend_vulkan_swapchain_destroy(struct backend_vulkan_swapchain* swapchain)
{
    for (u16 i = 0; i < swapchain->framebuffers.length; i += 1)
    {
        vkDestroyFramebuffer(swapchain->device->device, swapchain->framebuffers.data[i], ZZ_NULL);
        swapchain->framebuffers.data[i] = VK_NULL_HANDLE;
    }
    for (u16 i = 0; i < swapchain->imageViews.length; i += 1)
    {
        vkDestroyImageView(swapchain->device->device, swapchain->imageViews.data[i], ZZ_NULL);
        swapchain->imageViews.data[i] = VK_NULL_HANDLE;
    }
    vkDestroySwapchainKHR(swapchain->device->device, swapchain->swapchain, ZZ_NULL);
    swapchain->swapchain = VK_NULL_HANDLE;

    memory_array_destroy(&swapchain->framebuffers);
    memory_array_destroy(&swapchain->images);
    memory_array_destroy(&swapchain->imageViews);
}

b8 backend_vulkan_swapchain_recreate(struct backend_vulkan_swapchain* swapchain)
{
    vkDeviceWaitIdle(swapchain->device->device);
    backend_vulkan_swapchain_destroy(swapchain);
    struct backend_vulkan_swapchain_config swapchain_config;
    swapchain_config.surface = swapchain->surface;
    swapchain_config.device = swapchain->device;
    swapchain_config.render_pass = swapchain->render_pass;
    return backend_vulkan_swapchain_create(swapchain, &swapchain_config);
}

#endif