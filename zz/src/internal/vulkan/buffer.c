#if defined(ZZ__VULKAN)

#include "buffer.h"

#include "zz/log.h"

#include "command_pool.h"

b8 internal_vulkan_buffer_create(struct internal_vulkan_buffer* buffer, struct internal_vulkan_buffer_config* config)
{
    buffer->device = config->device;
    buffer->usageFlags = config->usageFlags;
    buffer->memoryPropertyFlags = config->memoryPropertyFlags;
    buffer->size = config->size;

    buffer->locked = ZZ_FALSE;

    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = ZZ_NULL;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = buffer->size;
    bufferCreateInfo.usage = buffer->usageFlags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = ZZ_NULL;

    if (vkCreateBuffer(buffer->device->device, &bufferCreateInfo, ZZ_NULL, &buffer->buffer) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(buffer->device->device, buffer->buffer, &memoryRequirements);

    if (!internal_vulkan_device_select_memory_type_index(buffer->device, &buffer->memoryTypeIndex, memoryRequirements.memoryTypeBits, buffer->memoryPropertyFlags))
    {
        return ZZ_FALSE;
    }

    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = ZZ_NULL;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = buffer->memoryTypeIndex;

    if (vkAllocateMemory(buffer->device->device, &memoryAllocateInfo, ZZ_NULL, &buffer->deviceMemory) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    vkBindBufferMemory(buffer->device->device, buffer->buffer, buffer->deviceMemory, 0);

    return ZZ_TRUE;
}

void internal_vulkan_buffer_destroy(struct internal_vulkan_buffer* buffer)
{
    vkFreeMemory(buffer->device->device, buffer->deviceMemory, ZZ_NULL);
    buffer->deviceMemory = VK_NULL_HANDLE;
    vkDestroyBuffer(buffer->device->device, buffer->buffer, ZZ_NULL);
    buffer->buffer = VK_NULL_HANDLE;
    buffer->usageFlags = 0;
    buffer->size = 0;
    buffer->locked = ZZ_FALSE;
}

b8 internal_vulkan_buffer_lock(struct internal_vulkan_buffer* buffer, void** block, u64 offset, u64 size, VkMemoryMapFlags flags)
{
    if (buffer->locked || vkMapMemory(buffer->device->device, buffer->deviceMemory, (VkDeviceSize)offset, (VkDeviceSize)size, flags, block) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }
    buffer->locked = ZZ_TRUE;
    return ZZ_TRUE;
}

void internal_vulkan_buffer_unlock(struct internal_vulkan_buffer* buffer)
{
    vkUnmapMemory(buffer->device->device, buffer->deviceMemory);
    buffer->locked = ZZ_FALSE;
}

b8 internal_vulkan_buffer_load(struct internal_vulkan_buffer* buffer, const void* data, u64 offset, u64 size, VkMemoryMapFlags flags)
{
    void* block = ZZ_NULL;
    if (!internal_vulkan_buffer_lock(buffer, &block, offset, size, flags))
    {
        return ZZ_FALSE;
    }
    memory_copy(block, data, size);

    internal_vulkan_buffer_unlock(buffer);

    return ZZ_TRUE;
}

#endif