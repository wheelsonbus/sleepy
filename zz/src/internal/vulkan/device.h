#if defined(ZZ__VULKAN)

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

struct internal_vulkan_device
{
    struct internal_vulkan_instance* instance;
    struct internal_vulkan_surface* surface;

    VkPhysicalDevice physicalDevice;
    struct queue_family_indices queue_family_indices;
    struct swapchain_support_details swapchain_support_details;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct internal_vulkan_device_config
{
    struct internal_vulkan_instance* instance;
    struct internal_vulkan_surface* surface;
};

b8 internal_vulkan_device_create(struct internal_vulkan_device* device, struct internal_vulkan_device_config* config);
void internal_vulkan_device_destroy(struct internal_vulkan_device* device);

b8 internal_vulkan_device_select_physical_device(struct internal_vulkan_device* device);

b8 internal_vulkan_device_select_memory_type_index(struct internal_vulkan_device* device, uint32_t* memoryTypeIndex, uint32_t memoryTypeIndexFilter, VkMemoryPropertyFlags memoryPropertyFlags);
#endif