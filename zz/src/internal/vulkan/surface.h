#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_SURFACE_H
#define ZZ_INTERNAL_VULKAN_SURFACE_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/application.h"

#include "instance.h"

struct zz_internal_vulkan_surface
{
    VkSurfaceKHR surface;

    struct zz_internal_vulkan_instance* instance;
};

struct zz_internal_vulkan_surface_config
{
    struct zz_internal_vulkan_instance* instance;
};

b8 zz_internal_vulkan_surface_create(struct zz_internal_vulkan_surface* surface, const struct zz_internal_vulkan_surface_config* config);
void zz_internal_vulkan_surface_destroy(struct zz_internal_vulkan_surface* surface);

#endif
#endif