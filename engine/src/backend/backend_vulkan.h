#pragma once

#include "zz.h"

#if defined(ZZ_BACKEND_VULKAN)

#include <vulkan/vulkan.h>

#include "core/assert/assert.h"

#define ZZ_BACKEND_VULKAN_ASSERT(expression) ZZ_ASSERT(expression == VK_SUCCESS);

struct backend_vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;

#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif
};

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData);
#elif defined(ZZ_RELEASE)
#endif

#endif
