#if defined(ZZ_BACKEND_VULKAN)

#include "instance.h"

#include "zz/log.h"

b8 backend_vulkan_create_instance(VkInstance* instance)
{
    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = "PLACEHOLDER";
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
    
    if (vkCreateInstance(&instanceCreateInfo, NULL, instance) != VK_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

void backend_vulkan_destroy_instance(VkInstance* instance)
{
    vkDestroyInstance(*instance, NULL);
    *instance = VK_NULL_HANDLE;
}

#endif