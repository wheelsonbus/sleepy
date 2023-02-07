#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_SYNC_H
#define ZZ_INTERNAL_VULKAN_SYNC_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

typedef struct {VkSemaphore* data; u16 length, capacity;} zz_memory_array_VkSemaphore_t;
typedef struct {VkFence* data; u16 length, capacity;} zz_memory_array_VkFence_t;

struct zz_internal_vulkan_sync
{
    struct zz_internal_vulkan_device* device;

    u16 max_frames_in_flight;
    zz_memory_array_VkSemaphore_t imageAvailableSemaphores;
    zz_memory_array_VkSemaphore_t renderFinishedSemaphores;
    zz_memory_array_VkFence_t inFlightFences;
};

struct zz_internal_vulkan_sync_config
{
    struct zz_internal_vulkan_device* device;

    u16 max_frames_in_flight;
};

b8 zz_internal_vulkan_sync_create(struct zz_internal_vulkan_sync* sync, struct zz_internal_vulkan_sync_config* config);
void zz_internal_vulkan_sync_destroy(struct zz_internal_vulkan_sync* sync);

#endif
#endif