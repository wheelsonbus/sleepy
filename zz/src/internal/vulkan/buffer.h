#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_BUFFER_H
#define ZZ_INTERNAL_VULKAN_BUFFER_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

struct zz_internal_vulkan_buffer
{
    VkBuffer buffer;

    struct zz_internal_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkDeviceMemory deviceMemory;
    uint32_t memoryTypeIndex;
    VkMemoryPropertyFlags memoryPropertyFlags;
    
    VkDeviceSize size;
    b8 locked;
};
typedef struct {struct zz_internal_vulkan_buffer* data; u16 length, capacity;} zz_memory_array_internal_vulkan_buffer_t;

struct zz_internal_vulkan_buffer_config
{
    struct zz_internal_vulkan_device* device;

    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;

    VkDeviceSize size;
};

b8 zz_internal_vulkan_buffer_create(struct zz_internal_vulkan_buffer* buffer, struct zz_internal_vulkan_buffer_config* config);
void zz_internal_vulkan_buffer_destroy(struct zz_internal_vulkan_buffer* buffer);

b8 zz_internal_vulkan_buffer_lock(struct zz_internal_vulkan_buffer* buffer, void** block, u64 offset, u64 size, VkMemoryMapFlags flags);
void zz_internal_vulkan_buffer_unlock(struct zz_internal_vulkan_buffer* buffer);

b8 zz_internal_vulkan_buffer_load(struct zz_internal_vulkan_buffer* buffer, const void* data, u64 offset, u64 size, VkMemoryMapFlags flags);

#endif
#endif