#if defined(ZZ__VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

struct internal_vulkan_buffer
{
    VkBuffer buffer;

    struct internal_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkDeviceMemory deviceMemory;
    uint32_t memoryTypeIndex;
    VkMemoryPropertyFlags memoryPropertyFlags;
    
    VkDeviceSize size;
    b8 locked;
};
typedef struct {struct internal_vulkan_buffer* data; u16 length, capacity;} memory_array_internal_vulkan_buffer_t;

struct internal_vulkan_buffer_config
{
    struct internal_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;

    VkDeviceSize size;
};

b8 internal_vulkan_buffer_create(struct internal_vulkan_buffer* buffer, struct internal_vulkan_buffer_config* config);
void internal_vulkan_buffer_destroy(struct internal_vulkan_buffer* buffer);

b8 internal_vulkan_buffer_lock(struct internal_vulkan_buffer* buffer, void** block, u64 offset, u64 size, VkMemoryMapFlags flags);
void internal_vulkan_buffer_unlock(struct internal_vulkan_buffer* buffer);

b8 internal_vulkan_buffer_load(struct internal_vulkan_buffer* buffer, const void* data, u64 offset, u64 size, VkMemoryMapFlags flags);

#endif