#ifdef ZZ_VULKAN

#include "manifest.h"

#include "zz/log.h"
#include "zz/filesystem.h"

b8 zz_internal_vulkan_manifest_create(struct zz_internal_vulkan_manifest* manifest, struct zz_internal_vulkan_manifest_config* config)
{
    manifest->device = config->device;

    zz_memory_array_create(&manifest->palettes);

    return ZZ_TRUE;
}

void zz_internal_vulkan_manifest_destroy(struct zz_internal_vulkan_manifest* manifest)
{
    while(manifest->palettes.length > 0) {
        vkDestroyImage(manifest->device->device, manifest->palettes.data[manifest->palettes.length - 1].image, ZZ_NULL); 
        zz_memory_array_pop(&manifest->palettes, ZZ_NULL);
    }

    zz_memory_array_destroy(&manifest->palettes);
}

b8 zz_internal_vulkan_manifest_load(struct zz_internal_vulkan_manifest* manifest, char* path)
{
    void* file = ZZ_NULL;
    if (!zz_filesystem_open(path, &file))
    {
        ZZ_LOG_ERROR("Failed to open file at path %s.", path);
        return ZZ_FALSE;
    }

    char line[256] = "";
    while (zz_filesystem_read_line(file, line, 256) && line[0] != '\0')
    {
        ZZ_LOG_DEBUG("%s", line);

        u64 size = zz_filesystem_get_size(line);
        void* data = zz_memory_allocate(size, ZZ_MEMORY_TAG_RENDER);
        zz_filesystem_load(line, data, size);

        u16 width = ZZ_FROM_BIG_ENDIAN_U16(((u16*)data)[0]);
        u16 height = ZZ_FROM_BIG_ENDIAN_U16(((u16*)data)[1]);
        ZZ_LOG_DEBUG("%dx%d", width, height);

        zz_memory_array_push(&manifest->palettes, &((struct zz_internal_vulkan_palette){0}));
        struct zz_internal_vulkan_palette* palette = &manifest->palettes.data[manifest->palettes.length - 1];

        VkImageCreateInfo imageCreateInfo;
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = ZZ_NULL;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UINT;
        imageCreateInfo.extent.width = (uint32_t)width;
        imageCreateInfo.extent.height = (uint32_t)height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.queueFamilyIndexCount = 0;
        imageCreateInfo.pQueueFamilyIndices = ZZ_NULL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        if (vkCreateImage(manifest->device->device, &imageCreateInfo, ZZ_NULL, &palette->image) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }

        return ZZ_TRUE;
    }

    zz_filesystem_close(file);

    return ZZ_TRUE;
}

#endif