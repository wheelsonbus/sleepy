#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "render.h"

b8 backend_vulkan_create_swapchain(VkSwapchainKHR* swapchain, struct queue_family_indices* queue_family_indices, struct swapchain_support_details* swapchain_support_details, VkSurfaceKHR surface, VkDevice device);

void backend_vulkan_destroy_swapchain(VkSwapchainKHR* swapchain, VkDevice device);

#endif