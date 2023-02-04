#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

struct backend_vulkan_buffer
{
    VkBuffer buffer;

    struct memory* memory;

    struct backend_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkDeviceMemory deviceMemory;
    uint32_t memoryTypeIndex;
    VkMemoryPropertyFlags memoryPropertyFlags;
    
    VkDeviceSize size;
    b8 locked;
};
typedef struct {struct backend_vulkan_buffer* data; u16 length, capacity;} memory_array_backend_vulkan_buffer_t;

struct backend_vulkan_buffer_config
{
    struct memory* memory;

    struct backend_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;

    VkDeviceSize size;
};

b8 backend_vulkan_buffer_create(struct backend_vulkan_buffer* buffer, struct backend_vulkan_buffer_config* config);
void backend_vulkan_buffer_destroy(struct backend_vulkan_buffer* buffer);

b8 backend_vulkan_buffer_lock(struct backend_vulkan_buffer* buffer, void** block, u64 offset, u64 size, VkMemoryMapFlags flags);
void backend_vulkan_buffer_unlock(struct backend_vulkan_buffer* buffer);

b8 backend_vulkan_buffer_load(struct backend_vulkan_buffer* buffer, const void* data, u64 offset, u64 size, VkMemoryMapFlags flags);

#endif