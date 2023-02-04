#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "instance.h"
#include "surface.h"

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

struct backend_vulkan_device
{
    struct backend_vulkan_instance* instance;
    struct backend_vulkan_surface* surface;

    VkPhysicalDevice physicalDevice;
    struct queue_family_indices queue_family_indices;
    struct swapchain_support_details swapchain_support_details;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct backend_vulkan_device_config
{
    struct backend_vulkan_instance* instance;
    struct backend_vulkan_surface* surface;
};

b8 backend_vulkan_device_create(struct backend_vulkan_device* device, struct backend_vulkan_device_config* config);
void backend_vulkan_device_destroy(struct backend_vulkan_device* device);

b8 backend_vulkan_device_select_physical_device(struct backend_vulkan_device* device);

b8 backend_vulkan_device_select_memory_type_index(struct backend_vulkan_device* device, uint32_t* memoryTypeIndex, uint32_t memoryTypeIndexFilter, VkMemoryPropertyFlags memoryPropertyFlags);
#endif