#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#if defined(ZZ_DEBUG)

b8 backend_vulkan_create_debug_utils_messenger(VkDebugUtilsMessengerEXT* debugUtilsMessenger, VkInstance instance);
void backend_vulkan_destroy_debug_utils_messenger(VkDebugUtilsMessengerEXT* debugUtilsMessenger, VkInstance instance);

VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

#elif defined(ZZ_RELEASE)
#endif

#endif