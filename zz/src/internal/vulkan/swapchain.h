#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_SWAPCHAIN_H
#define ZZ_INTERNAL_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/memory.h"
#include "zz/application.h"

#include "surface.h"
#include "device.h"
#include "render_pass.h"

typedef struct {VkImage* data; u16 length, capacity;} zz_memory_array_VkImage_t;
typedef struct {VkImageView* data; u16 length, capacity;} zz_memory_array_VkImageView_t;
typedef struct {VkFramebuffer* data; u16 length, capacity;} zz_memory_array_VkFramebuffer_t;

struct zz_internal_vulkan_swapchain
{
    VkSwapchainKHR swapchain;

    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_surface* surface;
    struct zz_internal_vulkan_render_pass* render_pass;

    zz_memory_array_VkImage_t images;
    zz_memory_array_VkImageView_t imageViews;
    zz_memory_array_VkFramebuffer_t framebuffers;
    VkExtent2D extent;
};

struct zz_internal_vulkan_swapchain_config
{
    struct zz_internal_vulkan_device* device;
    struct zz_internal_vulkan_surface* surface;
    struct zz_internal_vulkan_render_pass* render_pass;
};

b8 zz_internal_vulkan_swapchain_create(struct zz_internal_vulkan_swapchain* swapchain, const struct zz_internal_vulkan_swapchain_config* config);
void zz_internal_vulkan_swapchain_destroy(struct zz_internal_vulkan_swapchain* swapchain);

b8 zz_internal_vulkan_swapchain_recreate(struct zz_internal_vulkan_swapchain* swapchain);

#endif
#endif