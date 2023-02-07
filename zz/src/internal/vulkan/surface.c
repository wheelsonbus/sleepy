#ifdef ZZ_VULKAN

#include "surface.h"

#if defined(ZZ_WINDOWS)
#include "../src/internal/windows/application.h"
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ_LINUX)
#endif

b8 zz_internal_vulkan_surface_create(struct zz_internal_vulkan_surface* surface, const struct zz_internal_vulkan_surface_config* config)
{
    surface->instance = config->instance;
    
#if defined(ZZ_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = ZZ_NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = zz_internal_windows_application_get_hinstance();
    surfaceCreateInfo.hwnd = zz_internal_windows_application_get_hwnd();

    if (vkCreateWin32SurfaceKHR(surface->instance->instance, &surfaceCreateInfo, ZZ_NULL, &surface->surface) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }
#elif defined(ZZ_LINUX)
#endif
    
    return ZZ_TRUE;
}

void zz_internal_vulkan_surface_destroy(struct zz_internal_vulkan_surface* surface)
{
    vkDestroySurfaceKHR(surface->instance->instance, surface->surface, ZZ_NULL);
    surface->surface = ZZ_NULL;
}

#endif