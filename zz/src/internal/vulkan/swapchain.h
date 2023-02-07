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

typedef struct {VkImage* data; u16 length, capacity;} memory_array_VkImage_t; //memory_array_t(VkImage)
typedef struct {VkImageView* data; u16 length, capacity;} memory_array_VkImageView_t; //memory_array_t(VkImageView)
typedef struct {VkFramebuffer* data; u16 length, capacity;} memory_array_VkFramebuffer_t; //memory_array_t(VkFramebuffer)

struct internal_vulkan_swapchain
{
    VkSwapchainKHR swapchain;

    struct internal_vulkan_device* device;
    struct internal_vulkan_surface* surface;
    struct internal_vulkan_render_pass* render_pass;

    memory_array_VkImage_t images;
    memory_array_VkImageView_t imageViews;
    memory_array_VkFramebuffer_t framebuffers;
    VkExtent2D extent;
};

struct internal_vulkan_swapchain_config
{
    struct internal_vulkan_device* device;
    struct internal_vulkan_surface* surface;
    struct internal_vulkan_render_pass* render_pass;
};

b8 internal_vulkan_swapchain_create(struct internal_vulkan_swapchain* swapchain, const struct internal_vulkan_swapchain_config* config);
void internal_vulkan_swapchain_destroy(struct internal_vulkan_swapchain* swapchain);

b8 internal_vulkan_swapchain_recreate(struct internal_vulkan_swapchain* swapchain);

#endif
#endif