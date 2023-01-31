#if defined(ZZ_BACKEND_VULKAN)

#include "command_pool.h"

#include "zz/log.h"

b8 backend_vulkan_command_pool_create(struct backend_vulkan_command_pool* command_pool, struct backend_vulkan_command_pool_config* config)
{
    command_pool->memory = config->memory;
    command_pool->device = config->device;
    command_pool->sync = config->sync;

    VkCommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = NULL;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = command_pool->device->queue_family_indices.graphics;

    if (vkCreateCommandPool(command_pool->device->device, &commandPoolCreateInfo, NULL, &command_pool->commandPool) != VK_SUCCESS)
    {
        return FALSE;
    }

    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = NULL;
    commandBufferAllocateInfo.commandPool = command_pool->commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = command_pool->sync->max_frames_in_flight;

    memory_array_create_and_reserve(command_pool->memory, &command_pool->commandBuffers, command_pool->sync->max_frames_in_flight);
    if (vkAllocateCommandBuffers(command_pool->device->device, &commandBufferAllocateInfo, command_pool->commandBuffers.data) != VK_SUCCESS)
    {
        return FALSE;
    }
    command_pool->commandBuffers.length = command_pool->sync->max_frames_in_flight;

    return TRUE;
}

void backend_vulkan_command_pool_destroy(struct backend_vulkan_command_pool* command_pool)
{
    vkDestroyCommandPool(command_pool->device->device, command_pool->commandPool, NULL);
    command_pool->commandPool = VK_NULL_HANDLE;
}

b8 backend_vulkan_record_command_buffer(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkPipeline graphicsPipeline, memory_array_VkFramebuffer_t* swapchainFramebuffers, VkExtent2D* swapchainExtent, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = NULL;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = NULL;

    if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
    {
        return FALSE;
    }

    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = NULL;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = swapchainFramebuffers->data[imageIndex];
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent = *swapchainExtent;
    VkClearValue clearValue = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = swapchainExtent->width;
    viewport.height = swapchainExtent->height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 0.0f;
    
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = *swapchainExtent;

    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

#endif