#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_RENDER_PASS_H
#define ZZ_INTERNAL_VULKAN_RENDER_PASS_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "device.h"

struct internal_vulkan_render_pass
{
    VkRenderPass renderPass;

    struct internal_vulkan_device* device;
};

struct internal_vulkan_render_pass_config
{
    struct internal_vulkan_device* device;
};

b8 internal_vulkan_render_pass_create(struct internal_vulkan_render_pass* render_pass, struct internal_vulkan_render_pass_config* config);
void internal_vulkan_render_pass_destroy(struct internal_vulkan_render_pass* render_pass);

#endif
#endif