#if defined(ZZ__VULKAN)

#pragma once

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