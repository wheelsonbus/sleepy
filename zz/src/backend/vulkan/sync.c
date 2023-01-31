#if defined(ZZ_BACKEND_VULKAN)

#include "sync.h"

#include "zz/log.h"

b8 backend_vulkan_sync_create(struct backend_vulkan_sync* sync, struct backend_vulkan_sync_config* config)
{
    sync->memory = config->memory;
    sync->device = config->device;
    sync->max_frames_in_flight = config->max_frames_in_flight;

    memory_array_create_and_reserve(sync->memory, &sync->imageAvailableSemaphores, sync->max_frames_in_flight);
    memory_array_create_and_reserve(sync->memory, &sync->renderFinishedSemaphores, sync->max_frames_in_flight);
    memory_array_create_and_reserve(sync->memory, &sync->inFlightFences, sync->max_frames_in_flight);
    for (u16 i = 0; i < sync->max_frames_in_flight; i += 1)
    {
        VkSemaphoreCreateInfo imageAvailableSemaphoreCreateInfo;
        imageAvailableSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        imageAvailableSemaphoreCreateInfo.pNext = NULL;
        imageAvailableSemaphoreCreateInfo.flags = 0;

        if (vkCreateSemaphore(sync->device->device, &imageAvailableSemaphoreCreateInfo, NULL, &sync->imageAvailableSemaphores.data[i]) != VK_SUCCESS)
        {
            return FALSE;
        }
    
        VkSemaphoreCreateInfo renderFinishedSemaphoreCreateInfo;
        renderFinishedSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        renderFinishedSemaphoreCreateInfo.pNext = NULL;
        renderFinishedSemaphoreCreateInfo.flags = 0;

        if (vkCreateSemaphore(sync->device->device, &renderFinishedSemaphoreCreateInfo, NULL, &sync->renderFinishedSemaphores.data[i]) != VK_SUCCESS)
        {
            return FALSE;
        }

        VkFenceCreateInfo inFlightFenceCreateInfo;
        inFlightFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        inFlightFenceCreateInfo.pNext = NULL;
        inFlightFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(sync->device->device, &inFlightFenceCreateInfo, NULL, &sync->inFlightFences.data[i]) != VK_SUCCESS)
        {
            return FALSE;
        }
    }
    sync->imageAvailableSemaphores.length = sync->max_frames_in_flight;
    sync->renderFinishedSemaphores.length = sync->max_frames_in_flight;
    sync->inFlightFences.length = sync->max_frames_in_flight;

    return TRUE;
}

void backend_vulkan_sync_destroy(struct backend_vulkan_sync* sync)
{
    for (u16 i = 0; i < sync->max_frames_in_flight; i += 1)
    {
        vkDestroySemaphore(sync->device->device, sync->imageAvailableSemaphores.data[i], NULL);
        sync->imageAvailableSemaphores.data[i] = VK_NULL_HANDLE;
        vkDestroySemaphore(sync->device->device, sync->renderFinishedSemaphores.data[i], NULL);
        sync->renderFinishedSemaphores.data[i] = VK_NULL_HANDLE;
        vkDestroyFence(sync->device->device, sync->inFlightFences.data[i], NULL);
        sync->inFlightFences.data[i] = VK_NULL_HANDLE;
    }

    sync->max_frames_in_flight = 0;
}

#endif