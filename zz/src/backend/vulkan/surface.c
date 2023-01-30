#if defined(ZZ_BACKEND_VULKAN)

#include "surface.h"

#if defined(ZZ_PLATFORM_WINDOWS)
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ_PLATFORM_LINUX)
#endif

b8 backend_vulkan_create_surface(VkSurfaceKHR* surface, VkInstance instance, struct application* application)
{
    
#if defined(ZZ_PLATFORM_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = application->platform_application.hInstance;
    surfaceCreateInfo.hwnd = application->platform_application.hWnd;

    if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, surface) != VK_SUCCESS)
    {
        return FALSE;
    }
#elif defined(ZZ_PLATFORM_LINUX)
#endif
    
    return TRUE;
}

void backend_vulkan_destroy_surface(VkSurfaceKHR* surface, VkInstance instance)
{
    vkDestroySurfaceKHR(instance, *surface, NULL);
    *surface = NULL;
}

#endif