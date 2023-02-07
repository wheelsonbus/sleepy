#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_PIPELINE_H
#define ZZ_INTERNAL_VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"
#include "zz/math.h"

#include "vertex.h"
#include "device.h"
#include "render_pass.h"
#include "swapchain.h"
#include "buffer.h"

typedef struct {VkDescriptorSet* data; u16 length, capacity;} zz_memory_array_VkDescriptorSet_t;

struct zz_internal_vulkan_pipeline
{
    VkPipeline pipeline;

    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_render_pass* render_pass;
    struct zz_internal_vulkan_swapchain* swapchain;
    zz_memory_array_internal_vulkan_buffer_t* uniform_buffers;

    VkPipelineLayout pipelineLayout;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    zz_memory_array_VkDescriptorSet_t descriptorSets;
};

struct zz_internal_vulkan_pipeline_config
{
    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_render_pass* render_pass;
    struct zz_internal_vulkan_swapchain* swapchain;
    zz_memory_array_internal_vulkan_buffer_t* uniform_buffers;
};

b8 zz_internal_vulkan_pipeline_create(struct zz_internal_vulkan_pipeline* pipeline, struct zz_internal_vulkan_pipeline_config* config); 
void zz_internal_vulkan_pipeline_destroy(struct zz_internal_vulkan_pipeline* pipeline);

b8 zz_internal_vulkan_create_shader_module(VkShaderModule* shaderModule, VkDevice device, const char* filename);
void zz_internal_vulkan_destroy_shader_module(VkShaderModule* shaderModule, VkDevice device);

#endif
#endif