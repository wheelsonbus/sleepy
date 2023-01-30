#if defined(ZZ_BACKEND_VULKAN)

#include "backend/render.h"
#include "render.h"

#include <stdlib.h>

#include "zz/log.h"

#include "instance.h"
#include "debug.h"
#include "device.h"
#include "surface.h"
#include "swapchain.h"

b8 backend_render_create(struct backend_render* backend_render, struct backend_render_config* config)
{
    if (!backend_vulkan_create_instance(&backend_render->instance))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan instance.");
        return FALSE;
    }
    if (!backend_vulkan_create_debug_utils_messenger(&backend_render->debugUtilsMessenger, backend_render->instance))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan debug utils messenger.");
        return FALSE;
    }
    if (!backend_vulkan_create_surface(&backend_render->surface, backend_render->instance, config->application))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan surface.");
        return FALSE;
    }
    if (!backend_vulkan_select_physical_device(&backend_render->physicalDevice, &backend_render->queue_family_indices, &backend_render->swapchain_support_details, backend_render->instance, backend_render->surface))
    {
        ZZ_LOG_FATAL("Failed to select Vulkan physical device.");
        return FALSE;
    }
    if (!backend_vulkan_create_device(&backend_render->device, &backend_render->graphicsQueue, &backend_render->presentQueue, backend_render->physicalDevice, &backend_render->queue_family_indices))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan device.");
        return FALSE;
    }
    if (!backend_vulkan_create_swapchain(&backend_render->swapchain, &backend_render->queue_family_indices, &backend_render->swapchain_support_details, backend_render->surface, backend_render->device))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan swapchain.");
        return FALSE;
    }

    return TRUE;
}

void backend_render_destroy(struct backend_render* backend_render)
{
    backend_vulkan_destroy_swapchain(&backend_render->swapchain, backend_render->device);
    backend_vulkan_destroy_device(&backend_render->device);
    backend_vulkan_destroy_surface(&backend_render->surface, backend_render->instance);
    backend_vulkan_destroy_debug_utils_messenger(&backend_render->debugUtilsMessenger, backend_render->instance);
    backend_vulkan_destroy_instance(&backend_render->instance);
}

#endif