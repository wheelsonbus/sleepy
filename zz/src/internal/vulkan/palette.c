#ifdef ZZ_VULKAN

#include "zz/internal/palette.h"
#include "palette.h"

#include "render.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/filesystem.h"

b8 zz_internal_palette_create(struct zz_internal_palette* internal_palette, struct zz_internal_palette_config* config)
{
    internal_palette->data = zz_memory_allocate(sizeof(struct zz_internal_vulkan_palette), ZZ_MEMORY_TAG_ASSET);
    struct zz_internal_vulkan_palette* internal_vulkan_palette = (struct zz_internal_vulkan_palette*)internal_palette->data;

    u16 width = 0;
    u16 height = 0;

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

    ZZ_LOG_DEBUG("%d", internal_render.device.device);

    if (vkCreateImage(internal_render.device.device, &imageCreateInfo, ZZ_NULL, &internal_vulkan_palette->image) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    return ZZ_TRUE;
}

void zz_internal_palette_destroy(struct zz_internal_palette* internal_palette)
{
    zz_memory_deallocate(internal_palette->data, sizeof(struct zz_internal_vulkan_palette), ZZ_MEMORY_TAG_ASSET);
}

#endif