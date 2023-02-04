#if defined(ZZ_BACKEND_VULKAN)

#include "buffer.h"

#include "zz/log.h"

#include "command_pool.h"

b8 backend_vulkan_buffer_create(struct backend_vulkan_buffer* buffer, struct backend_vulkan_buffer_config* config)
{
    buffer->memory = config->memory;
    buffer->device = config->device;
    buffer->usageFlags = config->usageFlags;
    buffer->memoryPropertyFlags = config->memoryPropertyFlags;
    buffer->size = config->size;

    buffer->locked = FALSE;

    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = NULL;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = buffer->size;
    bufferCreateInfo.usage = buffer->usageFlags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = NULL;

    if (vkCreateBuffer(buffer->device->device, &bufferCreateInfo, NULL, &buffer->buffer) != VK_SUCCESS)
    {
        return FALSE;
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(buffer->device->device, buffer->buffer, &memoryRequirements);

    if (!backend_vulkan_device_select_memory_type_index(buffer->device, &buffer->memoryTypeIndex, memoryRequirements.memoryTypeBits, buffer->memoryPropertyFlags))
    {
        return FALSE;
    }

    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = NULL;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = buffer->memoryTypeIndex;

    if (vkAllocateMemory(buffer->device->device, &memoryAllocateInfo, NULL, &buffer->deviceMemory) != VK_SUCCESS)
    {
        return FALSE;
    }

    vkBindBufferMemory(buffer->device->device, buffer->buffer, buffer->deviceMemory, 0);

    return TRUE;
}

void backend_vulkan_buffer_destroy(struct backend_vulkan_buffer* buffer)
{
    vkFreeMemory(buffer->device->device, buffer->deviceMemory, NULL);
    buffer->deviceMemory = VK_NULL_HANDLE;
    vkDestroyBuffer(buffer->device->device, buffer->buffer, NULL);
    buffer->buffer = VK_NULL_HANDLE;
    buffer->usageFlags = 0;
    buffer->size = 0;
    buffer->locked = FALSE;
}

b8 backend_vulkan_buffer_lock(struct backend_vulkan_buffer* buffer, void** block, u64 offset, u64 size, VkMemoryMapFlags flags)
{
    if (buffer->locked || vkMapMemory(buffer->device->device, buffer->deviceMemory, (VkDeviceSize)offset, (VkDeviceSize)size, flags, block) != VK_SUCCESS)
    {
        return FALSE;
    }
    buffer->locked = TRUE;
    return TRUE;
}

void backend_vulkan_buffer_unlock(struct backend_vulkan_buffer* buffer)
{
    vkUnmapMemory(buffer->device->device, buffer->deviceMemory);
    buffer->locked = FALSE;
}

b8 backend_vulkan_buffer_load(struct backend_vulkan_buffer* buffer, const void* data, u64 offset, u64 size, VkMemoryMapFlags flags)
{
    void* block = NULL;
    if (!backend_vulkan_buffer_lock(buffer, &block, offset, size, flags))
    {
        return FALSE;
    }
    memory_copy(buffer->memory, block, data, size);

    backend_vulkan_buffer_unlock(buffer);

    return TRUE;
}

#endif