#if defined(ZZ_BACKEND_VULKAN)

#include "swapchain.h"

b8 backend_vulkan_create_swapchain(VkSwapchainKHR* swapchain, struct queue_family_indices* queue_family_indices, struct swapchain_support_details* swapchain_support_details, VkSurfaceKHR surface, VkDevice device)
{
    uint32_t imageCount = swapchain_support_details->surfaceCapabilities.minImageCount + 1;
    if (swapchain_support_details->surfaceCapabilities.maxImageCount != 0 && imageCount > swapchain_support_details->surfaceCapabilities.maxImageCount)
    {
        imageCount = swapchain_support_details->surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = NULL;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = swapchain_support_details->surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = swapchain_support_details->surfaceFormat.colorSpace;
    //swapchainCreateInfo.imageExtent = swapchain_support_details->surfaceCapabilities.currentExtent; // TODO Handle currentExtent.width/height == UINT32_MAX?
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    uint32_t queueFamilyIndices[2] = {queue_family_indices->graphics, queue_family_indices->present};
    if (queue_family_indices->graphics == queue_family_indices->present)
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = NULL;
    }
    else
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    swapchainCreateInfo.preTransform = swapchain_support_details->surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = swapchain_support_details->presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, swapchain) != VK_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

void backend_vulkan_destroy_swapchain(VkSwapchainKHR* swapchain, VkDevice device)
{
    vkDestroySwapchainKHR(device, *swapchain, NULL);
    *swapchain = VK_NULL_HANDLE;
}

#endif