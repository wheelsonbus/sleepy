#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_RENDER_PASS_H
#define ZZ_INTERNAL_VULKAN_RENDER_PASS_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "device.h"

struct zz_internal_vulkan_render_pass
{
    VkRenderPass renderPass;

    struct zz_internal_vulkan_device* device;
};

struct zz_internal_vulkan_render_pass_config
{
    struct zz_internal_vulkan_device* device;
};

b8 zz_internal_vulkan_render_pass_create(struct zz_internal_vulkan_render_pass* render_pass, struct zz_internal_vulkan_render_pass_config* config);
void zz_internal_vulkan_render_pass_destroy(struct zz_internal_vulkan_render_pass* render_pass);

#endif
#endif