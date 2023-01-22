#pragma once

#include "zz.h"

#if defined(ZZ_BACKEND_VULKAN)

#include <vulkan/vulkan.h>

#include "core/assert/assert.h"

#define ZZ_BACKEND_VULKAN_ASSERT(expression) ZZ_ASSERT(expression == VK_SUCCESS);

struct backend_vulkan_physical_device_swapchain_support_info
{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* surfaceFormats;
    u32 surface_format_count;
    VkPresentModeKHR* surfacePresentModes;
    u32 surface_present_mode_count;
};

struct backend_vulkan_physical_device_queue_family_info
{
    u32 graphics_index;
    u32 present_index;
    u32 compute_index;
    u32 transfer_index;
};

struct backend_vulkan_device
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

    VkDevice logicalDevice;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    struct backend_vulkan_physical_device_queue_family_info queue_family_info;
    struct backend_vulkan_physical_device_swapchain_support_info swapchain_support_info;
};

struct backend_vulkan_physical_device_requirements
{
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;

    b8 sampler_anisotropy;
    b8 discrete_gpu;

    const char** extension_names;
};

struct backend_vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;
    struct backend_vulkan_device device;

#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif
};

void backend_vulkan_physical_device_query_swapchain_support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info);
b8 backend_vulkan_physical_device_meets_requirements(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkPhysicalDeviceProperties* properties, const VkPhysicalDeviceFeatures* features, const struct backend_vulkan_physical_device_requirements* requirements, struct backend_vulkan_physical_device_queue_family_info* queue_family_info, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info);
b8 backend_vulkan_context_select_physical_device(struct backend_vulkan_context* context);
b8 backend_vulkan_context_create_device(struct backend_vulkan_context* context);
void backend_vulkan_context_destroy_device(struct backend_vulkan_context* context);

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData);
#elif defined(ZZ_RELEASE)
#endif

#endif
