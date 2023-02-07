#if defined(ZZ__VULKAN)

#include "surface.h"

#if defined(ZZ__WINDOWS)
#include "internal/windows/application.h"
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ__LINUX)
#endif

b8 internal_vulkan_surface_create(struct internal_vulkan_surface* surface, const struct internal_vulkan_surface_config* config)
{
    surface->instance = config->instance;
    
#if defined(ZZ__WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = ZZ_NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = internal_windows_application_get_hinstance();
    surfaceCreateInfo.hwnd = internal_windows_application_get_hwnd();

    if (vkCreateWin32SurfaceKHR(surface->instance->instance, &surfaceCreateInfo, ZZ_NULL, &surface->surface) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }
#elif defined(ZZ__LINUX)
#endif
    
    return ZZ_TRUE;
}

void internal_vulkan_surface_destroy(struct internal_vulkan_surface* surface)
{
    vkDestroySurfaceKHR(surface->instance->instance, surface->surface, ZZ_NULL);
    surface->surface = ZZ_NULL;
}

#endif