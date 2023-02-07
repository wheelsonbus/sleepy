#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_COMMAND_POOL_H
#define ZZ_INTERNAL_VULKAN_COMMAND_POOL_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"
#include "swapchain.h"
#include "sync.h"
#include "buffer.h"
#include "pipeline.h"

typedef struct {VkCommandBuffer* data; u16 length, capacity;} zz_memory_arrary_VkCommandBuffer_t;

struct zz_internal_vulkan_command_pool
{
    VkCommandPool commandPool;

    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_sync* sync;

    zz_memory_arrary_VkCommandBuffer_t commandBuffers;
};

struct zz_internal_vulkan_command_pool_config
{
    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_sync* sync;
};

b8 zz_internal_vulkan_command_pool_create(struct zz_internal_vulkan_command_pool* command_pool, struct zz_internal_vulkan_command_pool_config* config);
void zz_internal_vulkan_command_pool_destroy(struct zz_internal_vulkan_command_pool* command_pool);

b8 zz_internal_vulkan_command_pool_copy_buffer(struct zz_internal_vulkan_command_pool* command_pool, struct zz_internal_vulkan_buffer* destinationBuffer, struct zz_internal_vulkan_buffer* sourceBuffer, u64 size);

b8 zz_internal_vulkan_record_command_buffer(struct zz_internal_vulkan_buffer* vertex_buffer, struct zz_internal_vulkan_buffer* index_buffer, uint32_t indexCount, struct zz_internal_vulkan_pipeline* pipeline, u16 current_frame, VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkPipeline graphicsPipeline, zz_memory_array_VkFramebuffer_t* swapchainFramebuffers, VkExtent2D* swapchainExtent, uint32_t imageIndex);

#endif
#endif