#if defined(ZZ_BACKEND_VULKAN)

#pragma once

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

struct backend_vulkan_swapchain
{
    VkSwapchainKHR swapchain;

    struct backend_vulkan_device* device;
    struct backend_vulkan_surface* surface;
    struct backend_vulkan_render_pass* render_pass;

    memory_array_VkImage_t images;
    memory_array_VkImageView_t imageViews;
    memory_array_VkFramebuffer_t framebuffers;
    VkExtent2D extent;
};

struct backend_vulkan_swapchain_config
{
    struct backend_vulkan_device* device;
    struct backend_vulkan_surface* surface;
    struct backend_vulkan_render_pass* render_pass;
};

b8 backend_vulkan_swapchain_create(struct backend_vulkan_swapchain* swapchain, const struct backend_vulkan_swapchain_config* config);
void backend_vulkan_swapchain_destroy(struct backend_vulkan_swapchain* swapchain);

b8 backend_vulkan_swapchain_recreate(struct backend_vulkan_swapchain* swapchain);

#endif