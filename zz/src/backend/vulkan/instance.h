#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"


struct backend_vulkan_instance
{
    VkInstance instance;
#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif
};

struct backend_vulkan_instance_config
{

};

b8 backend_vulkan_instance_create(struct backend_vulkan_instance* instance, const struct backend_vulkan_instance_config* config);
void backend_vulkan_instance_destroy(struct backend_vulkan_instance* instance);

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_instance_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
#elif defined(ZZ_RELEASE)
#endif

#endif