#if defined(ZZ_BACKEND_VULKAN)

#include "surface.h"

#if defined(ZZ_PLATFORM_WINDOWS)
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ_PLATFORM_LINUX)
#endif

b8 backend_vulkan_surface_create(struct backend_vulkan_surface* surface, const struct backend_vulkan_surface_config* config)
{
    surface->instance = config->instance;
    
#if defined(ZZ_PLATFORM_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = config->application->platform_application.hInstance;
    surfaceCreateInfo.hwnd = config->application->platform_application.hWnd;

    if (vkCreateWin32SurfaceKHR(surface->instance->instance, &surfaceCreateInfo, NULL, &surface->surface) != VK_SUCCESS)
    {
        return FALSE;
    }
#elif defined(ZZ_PLATFORM_LINUX)
#endif
    
    return TRUE;
}

void backend_vulkan_surface_destroy(struct backend_vulkan_surface* surface)
{
    vkDestroySurfaceKHR(surface->instance->instance, surface->surface, NULL);
    surface->surface = NULL;
}

#endif