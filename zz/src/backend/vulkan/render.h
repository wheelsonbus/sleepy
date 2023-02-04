#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/camera.h"
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

typedef struct {u32* data; u16 length, capacity;} memory_array_u16_t;

struct backend_render
{
    struct memory* memory;
    struct application* application;

    struct backend_vulkan_instance instance;
    struct backend_vulkan_surface surface;
    struct backend_vulkan_device device;
    struct backend_vulkan_render_pass render_pass;
    struct backend_vulkan_swapchain swapchain;
    struct backend_vulkan_pipeline pipeline;
    struct backend_vulkan_sync sync;
    struct backend_vulkan_command_pool command_pool;

    struct backend_vulkan_buffer vertex_buffer;
    struct backend_vulkan_buffer vertex_staging_buffer;
    struct backend_vulkan_buffer index_buffer;
    struct backend_vulkan_buffer index_staging_buffer;
    memory_array_backend_vulkan_buffer_t uniform_buffers;

    memory_array_backend_vulkan_vertex_t vertices;
    memory_array_u16_t indices;

    struct camera* camera;

    b8 framebuffer_resized;

    u16 current_frame;
};

#endif