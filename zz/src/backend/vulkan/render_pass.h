#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "device.h"

struct backend_vulkan_render_pass
{
    VkRenderPass renderPass;

    struct backend_vulkan_device* device;
};

struct backend_vulkan_render_pass_config
{
    struct backend_vulkan_device* device;
};

b8 backend_vulkan_render_pass_create(struct backend_vulkan_render_pass* render_pass, struct backend_vulkan_render_pass_config* config);
void backend_vulkan_render_pass_destroy(struct backend_vulkan_render_pass* render_pass);

#endif