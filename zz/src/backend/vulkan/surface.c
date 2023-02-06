#if defined(ZZ_BACKEND_VULKAN)

#include "surface.h"

#if defined(ZZ_PLATFORM_WINDOWS)
#include "platform/windows/application.h"
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ_PLATFORM_LINUX)
#endif

b8 backend_vulkan_surface_create(struct backend_vulkan_surface* surface, const struct backend_vulkan_surface_config* config)
{
    surface->instance = config->instance;
    
#if defined(ZZ_PLATFORM_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = ZZ_NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = platform_windows_application_get_hinstance();
    surfaceCreateInfo.hwnd = platform_windows_application_get_hwnd();

    if (vkCreateWin32SurfaceKHR(surface->instance->instance, &surfaceCreateInfo, ZZ_NULL, &surface->surface) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }
#elif defined(ZZ_PLATFORM_LINUX)
#endif
    
    return ZZ_TRUE;
}

void backend_vulkan_surface_destroy(struct backend_vulkan_surface* surface)
{
    vkDestroySurfaceKHR(surface->instance->instance, surface->surface, ZZ_NULL);
    surface->surface = ZZ_NULL;
}

#endif