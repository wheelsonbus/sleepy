#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "render.h"

b8 backend_vulkan_select_physical_device(VkPhysicalDevice* physicalDevice, struct queue_family_indices* queue_family_indices, struct swapchain_support_details* swapchain_support_details, VkInstance instance, VkSurfaceKHR surface);

b8 backend_vulkan_create_device(VkDevice* device, VkQueue* graphicsQueue, VkQueue* presentQueue, VkPhysicalDevice physicalDevice, struct queue_family_indices* queue_family_indices);
void backend_vulkan_destroy_device(VkDevice* device);

#endif