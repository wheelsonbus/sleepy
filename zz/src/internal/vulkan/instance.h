#if defined(ZZ__VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"


struct internal_vulkan_instance
{
    VkInstance instance;
#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif
};

struct internal_vulkan_instance_config
{

};

b8 internal_vulkan_instance_create(struct internal_vulkan_instance* instance, const struct internal_vulkan_instance_config* config);
void internal_vulkan_instance_destroy(struct internal_vulkan_instance* instance);

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL internal_vulkan_instance_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
#elif defined(ZZ_RELEASE)
#endif

#endif