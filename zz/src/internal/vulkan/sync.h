#if defined(ZZ__VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

typedef struct {VkSemaphore* data; u16 length, capacity;} memory_array_VkSemaphore_t; //memory_array_t(VkSemaphore)
typedef struct {VkFence* data; u16 length, capacity;} memory_array_VkFence_t; //memory_array_t(VkFence)

struct internal_vulkan_sync
{
    struct internal_vulkan_device* device;

    u16 max_frames_in_flight;
    memory_array_VkSemaphore_t imageAvailableSemaphores;
    memory_array_VkSemaphore_t renderFinishedSemaphores;
    memory_array_VkFence_t inFlightFences;
};

struct internal_vulkan_sync_config
{
    struct internal_vulkan_device* device;

    u16 max_frames_in_flight;
};

b8 internal_vulkan_sync_create(struct internal_vulkan_sync* sync, struct internal_vulkan_sync_config* config);
void internal_vulkan_sync_destroy(struct internal_vulkan_sync* sync);

#endif