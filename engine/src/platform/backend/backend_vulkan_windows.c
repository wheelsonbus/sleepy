#include "backend_vulkan.h"
#include "backend_vulkan_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

#include <windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "platform/core/application/application.h"
#include "platform/core/application/application_windows.h"
#include "backend/backend_vulkan.h"

void platform_backend_vulkan_dynamic_array_push_required_extension_names(const char*** dynamic_array)
{
    dynamic_array_push(*dynamic_array, &"VK_KHR_win32_surface");
}

b8 platform_backend_vulkan_context_create_surface(struct platform_application* application, struct backend_vulkan_context* context)
{
    struct platform_application_windows_state* state = (struct platform_application_windows_state*)application->state;

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    surfaceCreateInfo.hinstance = state->hInstance;
    surfaceCreateInfo.hwnd = state->hWnd;

    ZZ_BACKEND_VULKAN_ASSERT(vkCreateWin32SurfaceKHR(context->instance, &surfaceCreateInfo, context->allocator, &context->surface));

    return TRUE;
}

#endif