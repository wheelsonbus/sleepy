#if defined(ZZ_BACKEND_VULKAN)

#include "instance.h"

#include "zz/log.h"

b8 backend_vulkan_instance_create(struct backend_vulkan_instance* instance, const struct backend_vulkan_instance_config* config)
{
    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = "PLACEHOLDER"; // PLACEHOLDER
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // PLACEHOLDER
    applicationInfo.pEngineName = "Sleepy Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(ZZ_VERSION_MAJOR, ZZ_VERSION_MINOR, ZZ_VERSION_PATCH);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
#if defined(ZZ_DEBUG)
    instanceCreateInfo.enabledLayerCount = 1;
    const char* enabledLayerNames[1] = {"VK_LAYER_KHRONOS_validation"};
    instanceCreateInfo.ppEnabledLayerNames = enabledLayerNames;
#if defined(ZZ_PLATFORM_WINDOWS)
    instanceCreateInfo.enabledExtensionCount = 3;
    const char* enabledExtensionNames[3] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME, "VK_KHR_surface", "VK_KHR_win32_surface"};
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
#elif defined(ZZ_PLATFORM_LINUX)
#endif
#elif defined(ZZ_RELEASE)
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = NULL;
#if defined(ZZ_PLATFORM_WINDOWS)
    instanceCreateInfo.enabledExtensionCount = 2;
    const char* enabledExtensionNames[2] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
#elif defined(ZZ_PLATFORM_LINUX)
#endif
#endif
    
    if (vkCreateInstance(&instanceCreateInfo, NULL, &instance->instance) != VK_SUCCESS)
    {
        return FALSE;
    }

    
#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;
    debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsMessengerCreateInfo.pNext = NULL;
    debugUtilsMessengerCreateInfo.flags = 0;
    debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsMessengerCreateInfo.pfnUserCallback = backend_vulkan_instance_debug_callback;
    debugUtilsMessengerCreateInfo.pUserData = NULL;

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance->instance, "vkCreateDebugUtilsMessengerEXT");
    if (vkCreateDebugUtilsMessengerEXT == NULL)
    {
        return FALSE;
    }
    if (vkCreateDebugUtilsMessengerEXT(instance->instance, &debugUtilsMessengerCreateInfo, NULL, &instance->debugUtilsMessenger) != VK_SUCCESS)
    {
        return FALSE;
    }
#elif defined(ZZ_RELEASE)
#endif

    return TRUE;
}

void backend_vulkan_instance_destroy(struct backend_vulkan_instance* instance)
{
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance->instance, "vkDestroyDebugUtilsMessengerEXT");
    vkDestroyDebugUtilsMessengerEXT(instance->instance, instance->debugUtilsMessenger, NULL);
    instance->debugUtilsMessenger = VK_NULL_HANDLE;

    vkDestroyInstance(instance->instance, NULL);
    instance->instance = VK_NULL_HANDLE;
}

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_instance_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch(messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ZZ_LOG_ERROR(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            ZZ_LOG_WARNING(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            ZZ_LOG_INFO(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            ZZ_LOG_TRACE(pCallbackData->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}
#elif defined(ZZ_RELEASE)
#endif

#endif