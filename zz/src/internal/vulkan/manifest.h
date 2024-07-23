#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_MANIFEST_H
#define ZZ_INTERNAL_VULKAN_MANIFEST_H

#include "zz/zz.h"
#include "zz/memory.h"

#include "device.h"

struct zz_internal_vulkan_palette
{
    VkImage image;
};
typedef struct {struct zz_internal_vulkan_palette* data; u16 length, capacity;} zz_memory_array_internal_vulkan_palette_t;

struct zz_internal_vulkan_manifest
{
    struct zz_internal_vulkan_device* device;
    
    zz_memory_array_internal_vulkan_palette_t palettes;
};

struct zz_internal_vulkan_manifest_config
{
    struct zz_internal_vulkan_device* device;
};

b8 zz_internal_vulkan_manifest_create(struct zz_internal_vulkan_manifest* manifest, struct zz_internal_vulkan_manifest_config* config);
void zz_internal_vulkan_manifest_destroy(struct zz_internal_vulkan_manifest* manifest);

b8 zz_internal_vulkan_manifest_load(struct zz_internal_vulkan_manifest* manifest, char* path);

#endif
#endif