#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

typedef struct {VkSemaphore* data; u16 length, capacity;} memory_array_VkSemaphore_t; //memory_array_t(VkSemaphore)
typedef struct {VkFence* data; u16 length, capacity;} memory_array_VkFence_t; //memory_array_t(VkFence)

struct backend_vulkan_sync
{
    struct memory* memory;

    struct backend_vulkan_device* device;

    u16 max_frames_in_flight;
    memory_array_VkSemaphore_t imageAvailableSemaphores;
    memory_array_VkSemaphore_t renderFinishedSemaphores;
    memory_array_VkFence_t inFlightFences;
};

struct backend_vulkan_sync_config
{
    struct memory* memory;

    struct backend_vulkan_device* device;

    u16 max_frames_in_flight;
};

b8 backend_vulkan_sync_create(struct backend_vulkan_sync* sync, struct backend_vulkan_sync_config* config);
void backend_vulkan_sync_destroy(struct backend_vulkan_sync* sync);

#endif