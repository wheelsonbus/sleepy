#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_DEVICE_H
#define ZZ_INTERNAL_VULKAN_DEVICE_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "instance.h"
#include "surface.h"

struct zz_queue_family_indices
{
    uint32_t graphics, present;
};

struct zz_swapchain_support_details
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
};

struct zz_internal_vulkan_device
{
    struct zz_internal_vulkan_instance* instance;
    struct zz_internal_vulkan_surface* surface;

    VkPhysicalDevice physicalDevice;
    struct zz_queue_family_indices queue_family_indices;
    struct zz_swapchain_support_details swapchain_support_details;

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct zz_internal_vulkan_device_config
{
    struct zz_internal_vulkan_instance* instance;
    struct zz_internal_vulkan_surface* surface;
};

b8 zz_internal_vulkan_device_create(struct zz_internal_vulkan_device* device, struct zz_internal_vulkan_device_config* config);
void zz_internal_vulkan_device_destroy(struct zz_internal_vulkan_device* device);

b8 zz_internal_vulkan_device_select_physical_device(struct zz_internal_vulkan_device* device);

b8 zz_internal_vulkan_device_select_memory_type_index(struct zz_internal_vulkan_device* device, uint32_t* memoryTypeIndex, uint32_t memoryTypeIndexFilter, VkMemoryPropertyFlags memoryPropertyFlags);

#endif
#endif