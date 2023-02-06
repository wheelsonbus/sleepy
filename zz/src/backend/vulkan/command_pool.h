#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"
#include "swapchain.h"
#include "sync.h"
#include "buffer.h"
#include "pipeline.h"

typedef struct {VkCommandBuffer* data; u16 length, capacity;} memory_arrary_VkCommandBuffer_t; //memory_array_t(VkCommandBuffer)

struct backend_vulkan_command_pool
{
    VkCommandPool commandPool;

    struct backend_vulkan_device* device;
    struct backend_vulkan_sync* sync;

    memory_arrary_VkCommandBuffer_t commandBuffers;
};

struct backend_vulkan_command_pool_config
{
    struct backend_vulkan_device* device;
    struct backend_vulkan_sync* sync;
};

b8 backend_vulkan_command_pool_create(struct backend_vulkan_command_pool* command_pool, struct backend_vulkan_command_pool_config* config);
void backend_vulkan_command_pool_destroy(struct backend_vulkan_command_pool* command_pool);

b8 backend_vulkan_command_pool_copy_buffer(struct backend_vulkan_command_pool* command_pool, struct backend_vulkan_buffer* destinationBuffer, struct backend_vulkan_buffer* sourceBuffer, u64 size);

b8 backend_vulkan_record_command_buffer(struct backend_vulkan_buffer* vertex_buffer, struct backend_vulkan_buffer* index_buffer, uint32_t indexCount, struct backend_vulkan_pipeline* pipeline, u16 current_frame, VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkPipeline graphicsPipeline, memory_array_VkFramebuffer_t* swapchainFramebuffers, VkExtent2D* swapchainExtent, uint32_t imageIndex);

#endif