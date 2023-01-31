#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/application.h"

#include "instance.h"

struct backend_vulkan_surface
{
    VkSurfaceKHR surface;

    struct backend_vulkan_instance* instance;
};

struct backend_vulkan_surface_config
{
    struct application* application;
    
    struct backend_vulkan_instance* instance;
};

b8 backend_vulkan_surface_create(struct backend_vulkan_surface* surface, const struct backend_vulkan_surface_config* config);
void backend_vulkan_surface_destroy(struct backend_vulkan_surface* surface);

#endif