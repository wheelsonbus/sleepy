#if defined(ZZ__VULKAN)

#include "sync.h"

#include "zz/log.h"

b8 internal_vulkan_sync_create(struct internal_vulkan_sync* sync, struct internal_vulkan_sync_config* config)
{
    sync->device = config->device;
    sync->max_frames_in_flight = config->max_frames_in_flight;

    memory_array_create_and_reserve(&sync->imageAvailableSemaphores, sync->max_frames_in_flight);
    memory_array_create_and_reserve(&sync->renderFinishedSemaphores, sync->max_frames_in_flight);
    memory_array_create_and_reserve(&sync->inFlightFences, sync->max_frames_in_flight);
    for (u16 i = 0; i < sync->max_frames_in_flight; i += 1)
    {
        VkSemaphoreCreateInfo imageAvailableSemaphoreCreateInfo;
        imageAvailableSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        imageAvailableSemaphoreCreateInfo.pNext = ZZ_NULL;
        imageAvailableSemaphoreCreateInfo.flags = 0;

        if (vkCreateSemaphore(sync->device->device, &imageAvailableSemaphoreCreateInfo, ZZ_NULL, &sync->imageAvailableSemaphores.data[i]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }
    
        VkSemaphoreCreateInfo renderFinishedSemaphoreCreateInfo;
        renderFinishedSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        renderFinishedSemaphoreCreateInfo.pNext = ZZ_NULL;
        renderFinishedSemaphoreCreateInfo.flags = 0;

        if (vkCreateSemaphore(sync->device->device, &renderFinishedSemaphoreCreateInfo, ZZ_NULL, &sync->renderFinishedSemaphores.data[i]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }

        VkFenceCreateInfo inFlightFenceCreateInfo;
        inFlightFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        inFlightFenceCreateInfo.pNext = ZZ_NULL;
        inFlightFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(sync->device->device, &inFlightFenceCreateInfo, ZZ_NULL, &sync->inFlightFences.data[i]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }
    }
    sync->imageAvailableSemaphores.length = sync->max_frames_in_flight;
    sync->renderFinishedSemaphores.length = sync->max_frames_in_flight;
    sync->inFlightFences.length = sync->max_frames_in_flight;

    return ZZ_TRUE;
}

void internal_vulkan_sync_destroy(struct internal_vulkan_sync* sync)
{
    for (u16 i = 0; i < sync->max_frames_in_flight; i += 1)
    {
        vkDestroySemaphore(sync->device->device, sync->imageAvailableSemaphores.data[i], ZZ_NULL);
        sync->imageAvailableSemaphores.data[i] = VK_NULL_HANDLE;
        vkDestroySemaphore(sync->device->device, sync->renderFinishedSemaphores.data[i], ZZ_NULL);
        sync->renderFinishedSemaphores.data[i] = VK_NULL_HANDLE;
        vkDestroyFence(sync->device->device, sync->inFlightFences.data[i], ZZ_NULL);
        sync->inFlightFences.data[i] = VK_NULL_HANDLE;
    }

    memory_array_destroy(&sync->imageAvailableSemaphores);
    memory_array_destroy(&sync->renderFinishedSemaphores);
    memory_array_destroy(&sync->inFlightFences);

    sync->max_frames_in_flight = 0;
}

#endif