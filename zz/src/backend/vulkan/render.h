#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

struct queue_family_indices
{
    uint32_t graphics, present;
};

struct swapchain_support_details
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
};

struct backend_render
{
    VkInstance instance;

#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif

    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice;
    struct queue_family_indices queue_family_indices;
    struct swapchain_support_details swapchain_support_details;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapchain;
};

struct backend_vulkan_render
{
    VkInstance instance;
};

#endif