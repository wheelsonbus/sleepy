#if defined(ZZ__VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/application.h"

#include "instance.h"

struct internal_vulkan_surface
{
    VkSurfaceKHR surface;

    struct internal_vulkan_instance* instance;
};

struct internal_vulkan_surface_config
{
    struct internal_vulkan_instance* instance;
};

b8 internal_vulkan_surface_create(struct internal_vulkan_surface* surface, const struct internal_vulkan_surface_config* config);
void internal_vulkan_surface_destroy(struct internal_vulkan_surface* surface);

#endif