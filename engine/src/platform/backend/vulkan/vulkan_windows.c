#include "vulkan.h"
#include "vulkan_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

void platform_backend_vulkan_dynamic_array_push_required_extension_names(const char*** dynamic_array)
{
    dynamic_array_push(*dynamic_array, &"VK_KHR_win32_surface");
}

#endif