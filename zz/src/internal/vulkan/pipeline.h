#if defined(ZZ__VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"
#include "zz/math.h"

#include "vertex.h"
#include "device.h"
#include "render_pass.h"
#include "swapchain.h"
#include "buffer.h"

typedef struct {VkDescriptorSet* data; u16 length, capacity;} memory_array_VkDescriptorSet_t;

struct internal_vulkan_pipeline
{
    VkPipeline pipeline;

    struct internal_vulkan_device* device;
    struct internal_vulkan_render_pass* render_pass;
    struct internal_vulkan_swapchain* swapchain;
    memory_array_internal_vulkan_buffer_t* uniform_buffers;

    VkPipelineLayout pipelineLayout;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    memory_array_VkDescriptorSet_t descriptorSets;
};

struct internal_vulkan_pipeline_config
{
    struct internal_vulkan_device* device;
    struct internal_vulkan_render_pass* render_pass;
    struct internal_vulkan_swapchain* swapchain;
    memory_array_internal_vulkan_buffer_t* uniform_buffers;
};

b8 internal_vulkan_pipeline_create(struct internal_vulkan_pipeline* pipeline, struct internal_vulkan_pipeline_config* config); 
void internal_vulkan_pipeline_destroy(struct internal_vulkan_pipeline* pipeline);

b8 internal_vulkan_create_shader_module(VkShaderModule* shaderModule, VkDevice device, const char* filename);
void internal_vulkan_destroy_shader_module(VkShaderModule* shaderModule, VkDevice device);

#endif