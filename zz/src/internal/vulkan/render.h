#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VULKAN_RENDER_H
#define ZZ_INTERNAL_VULKAN_RENDER_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/sprite.h"

#include "instance.h"
#include "surface.h"
#include "device.h"
#include "render_pass.h"
#include "swapchain.h"
#include "pipeline.h"
#include "command_pool.h"
#include "sync.h"
#include "buffer.h"

typedef struct {u32* data; u16 length, capacity;} zz_memory_array_u32_t;

struct zz_internal_render
{
    struct zz_internal_vulkan_instance instance;
    struct zz_internal_vulkan_surface surface;
    struct zz_internal_vulkan_device device;
    struct zz_internal_vulkan_render_pass render_pass;
    struct zz_internal_vulkan_swapchain swapchain;
    struct zz_internal_vulkan_pipeline pipeline;
    struct zz_internal_vulkan_sync sync;
    struct zz_internal_vulkan_command_pool command_pool;

    struct zz_internal_vulkan_buffer vertex_buffer;
    struct zz_internal_vulkan_buffer vertex_staging_buffer;
    struct zz_internal_vulkan_buffer index_buffer;
    struct zz_internal_vulkan_buffer index_staging_buffer;
    zz_memory_array_internal_vulkan_vertex_t vertices;
    zz_memory_array_u32_t indices;

    zz_memory_array_internal_vulkan_buffer_t uniform_buffers;
    struct zz_internal_vulkan_uniform_buffer_object uniform_buffer_object;

    b8 framebuffer_resized;

    u16 current_frame;
};

#endif
#endif