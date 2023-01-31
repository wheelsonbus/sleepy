#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"

#include "device.h"
#include "render_pass.h"
#include "swapchain.h"

// TODO Remove typedef struct {char* data; u16 length, capacity;} memory_array_char_t;

struct backend_vulkan_pipeline
{
    VkPipeline pipeline;

    struct backend_vulkan_device* device;
    struct backend_vulkan_render_pass* render_pass;
    struct backend_vulkan_swapchain* swapchain;

    VkPipelineLayout pipelineLayout;
};

struct backend_vulkan_pipeline_config
{
    struct backend_vulkan_device* device;
    struct backend_vulkan_render_pass* render_pass;
    struct backend_vulkan_swapchain* swapchain;
};

b8 backend_vulkan_pipeline_create(struct backend_vulkan_pipeline* pipeline, struct backend_vulkan_pipeline_config* config); 
void backend_vulkan_pipeline_destroy(struct backend_vulkan_pipeline* pipeline);

b8 backend_vulkan_create_shader_module(VkShaderModule* shaderModule, VkDevice device, const char* filename);
void backend_vulkan_destroy_shader_module(VkShaderModule* shaderModule, VkDevice device);

#endif