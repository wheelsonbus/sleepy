#ifdef ZZ_VULKAN

#include "zz/internal/render.h"
#include "render.h"

#include "zz/log.h"
#include "zz/application.h"

static struct zz_internal_render internal_render;

static struct zz_internal_vulkan_manifest manifest;

b8 zz_internal_render_initialize(struct zz_internal_render_config* config)
{
    internal_render.framebuffer_resized = ZZ_FALSE;
    internal_render.current_frame = 0;

    internal_render.uniform_buffer_object.model = mat4_identity();
    internal_render.uniform_buffer_object.view = mat4_identity();
    internal_render.uniform_buffer_object.projection = mat4_identity();

    zz_memory_array_create_and_reserve(&internal_render.vertices, 1024);
    zz_memory_array_create_and_reserve(&internal_render.indices, 1024);

    struct zz_internal_vulkan_instance_config instance_config;
    if (!zz_internal_vulkan_instance_create(&internal_render.instance, &instance_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan instance.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_surface_config surface_config;
    surface_config.instance = &internal_render.instance;
    if (!zz_internal_vulkan_surface_create(&internal_render.surface, &surface_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan surface.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_device_config device_config;
    device_config.instance = &internal_render.instance;
    device_config.surface = &internal_render.surface;
    if (!zz_internal_vulkan_device_create(&internal_render.device, &device_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan device.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_render_pass_config render_pass_config;
    render_pass_config.device = &internal_render.device;
    if (!zz_internal_vulkan_render_pass_create(&internal_render.render_pass, &render_pass_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan render pass.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_swapchain_config swapchain_config;
    swapchain_config.surface = &internal_render.surface;
    swapchain_config.device = &internal_render.device;
    swapchain_config.render_pass = &internal_render.render_pass;
    if (!zz_internal_vulkan_swapchain_create(&internal_render.swapchain, &swapchain_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan swapchain.");
        return ZZ_FALSE;
    }
    
    struct zz_internal_vulkan_sync_config sync_config;
    sync_config.device = &internal_render.device;
    sync_config.max_frames_in_flight = 2;
    if (!zz_internal_vulkan_sync_create(&internal_render.sync, &sync_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan sync.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_buffer_config vertex_buffer_config;
    vertex_buffer_config.device = &internal_render.device;
    vertex_buffer_config.usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertex_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    vertex_buffer_config.size = sizeof(struct zz_internal_vulkan_vertex) * 1024;
    if (!zz_internal_vulkan_buffer_create(&internal_render.vertex_buffer, &vertex_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_buffer_config vertex_staging_buffer_config;
    vertex_staging_buffer_config.device = &internal_render.device;
    vertex_staging_buffer_config.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    vertex_staging_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    vertex_staging_buffer_config.size = sizeof(struct zz_internal_vulkan_vertex) * 1024;
    if (!zz_internal_vulkan_buffer_create(&internal_render.vertex_staging_buffer, &vertex_staging_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return ZZ_FALSE;
    }
    
    struct zz_internal_vulkan_buffer_config index_buffer_config;
    index_buffer_config.device = &internal_render.device;
    index_buffer_config.usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    index_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    index_buffer_config.size = sizeof(u32) * 1024;
    if (!zz_internal_vulkan_buffer_create(&internal_render.index_buffer, &index_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return ZZ_FALSE;
    }
    
    struct zz_internal_vulkan_buffer_config index_staging_buffer_config;
    index_staging_buffer_config.device = &internal_render.device;
    index_staging_buffer_config.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    index_staging_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    index_staging_buffer_config.size = sizeof(u32) * 1024;
    if (!zz_internal_vulkan_buffer_create(&internal_render.index_staging_buffer, &index_staging_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return ZZ_FALSE;
    }

    zz_memory_array_create_and_reserve(&internal_render.uniform_buffers, internal_render.sync.max_frames_in_flight);
    for (u16 i = 0; i < internal_render.uniform_buffers.capacity; i += 1)
    {
        struct zz_internal_vulkan_buffer_config uniform_buffer_config;
        uniform_buffer_config.device = &internal_render.device;
        uniform_buffer_config.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        uniform_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        uniform_buffer_config.size = sizeof(struct zz_internal_vulkan_uniform_buffer_object);
        if (!zz_internal_vulkan_buffer_create(&internal_render.uniform_buffers.data[i], &uniform_buffer_config)) // TODO Implement persistent mapping
        {
            ZZ_LOG_FATAL("Failed to create Vulkan uniform buffer.");
            return ZZ_FALSE;
        }
    }
    internal_render.uniform_buffers.length = internal_render.uniform_buffers.capacity;

    struct zz_internal_vulkan_pipeline_config pipeline_config;
    pipeline_config.device = &internal_render.device;
    pipeline_config.render_pass = &internal_render.render_pass;
    pipeline_config.swapchain = &internal_render.swapchain;
    pipeline_config.uniform_buffers = &internal_render.uniform_buffers;
    if (!zz_internal_vulkan_pipeline_create(&internal_render.pipeline, &pipeline_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan pipeline.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_command_pool_config command_pool_config;
    command_pool_config.device = &internal_render.device;
    command_pool_config.sync = &internal_render.sync;
    if (!zz_internal_vulkan_command_pool_create(&internal_render.command_pool, &command_pool_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan command pool.");
        return ZZ_FALSE;
    }

    struct zz_internal_vulkan_manifest_config manifest_config;
    manifest_config.device = &internal_render.device;
    if (!zz_internal_vulkan_manifest_create(&manifest, &manifest_config))
    {
        ZZ_LOG_FATAL("Failed to create render manifest.");
        return ZZ_FALSE;
    }
    zz_internal_vulkan_manifest_load(&manifest, "four.manifest");

    return ZZ_TRUE;
}

void zz_internal_render_deinitialize()
{
    vkDeviceWaitIdle(internal_render.device.device);

    zz_internal_vulkan_command_pool_destroy(&internal_render.command_pool);
    zz_internal_vulkan_pipeline_destroy(&internal_render.pipeline);
    for (u16 i = 0; i < internal_render.uniform_buffers.length; i += 1)
    {
        zz_internal_vulkan_buffer_destroy(&internal_render.uniform_buffers.data[i]);
    }
    zz_memory_array_destroy(&internal_render.uniform_buffers);
    zz_internal_vulkan_buffer_destroy(&internal_render.index_staging_buffer);
    zz_internal_vulkan_buffer_destroy(&internal_render.index_buffer);
    zz_internal_vulkan_buffer_destroy(&internal_render.vertex_staging_buffer);
    zz_internal_vulkan_buffer_destroy(&internal_render.vertex_buffer);
    zz_internal_vulkan_sync_destroy(&internal_render.sync);
    zz_internal_vulkan_swapchain_destroy(&internal_render.swapchain);
    zz_internal_vulkan_render_pass_destroy(&internal_render.render_pass);
    zz_internal_vulkan_device_destroy(&internal_render.device);
    zz_internal_vulkan_surface_destroy(&internal_render.surface);
    zz_internal_vulkan_instance_destroy(&internal_render.instance);

    zz_memory_array_destroy(&internal_render.indices);
    zz_memory_array_destroy(&internal_render.vertices);
}

void zz_internal_render_resize()
{
    u16 width, height;
    zz_application_get_size(&width, &height);
    if ((uint32_t)width != internal_render.swapchain.extent.width || (uint32_t)height != internal_render.swapchain.extent.height)
    {
        internal_render.framebuffer_resized = ZZ_TRUE;
    }
}

b8 zz_internal_render_draw_frame()
{
    if (internal_render.framebuffer_resized)
    {
        ZZ_LOG_INFO("Recreating Vulkan swapchain.");
        zz_internal_vulkan_swapchain_recreate(&internal_render.swapchain);
        internal_render.framebuffer_resized = ZZ_FALSE;
        return ZZ_FALSE; 
    }

    if (internal_render.vertices.length != 0 && internal_render.indices.length != 0)
    {
        vkWaitForFences(internal_render.device.device, 1, &internal_render.sync.inFlightFences.data[internal_render.current_frame], VK_TRUE, (uint64_t)-1);

        uint32_t imageIndex;
        VkResult acquireNextImageResult = vkAcquireNextImageKHR(internal_render.device.device, internal_render.swapchain.swapchain, (uint64_t)-1, internal_render.sync.imageAvailableSemaphores.data[internal_render.current_frame], VK_NULL_HANDLE, &imageIndex);
        if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR)
        {
            ZZ_LOG_INFO("Recreating Vulkan swapchain.");
            zz_internal_vulkan_swapchain_recreate(&internal_render.swapchain);
            return ZZ_FALSE;
        }
        else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR)
        {
            return ZZ_FALSE;
        }
        
        vkResetFences(internal_render.device.device, 1, &internal_render.sync.inFlightFences.data[internal_render.current_frame]);

        zz_internal_vulkan_buffer_load(&internal_render.vertex_staging_buffer, internal_render.vertices.data, 0, sizeof(internal_render.vertices.data[0]) * internal_render.vertices.length, 0);
        zz_internal_vulkan_command_pool_copy_buffer(&internal_render.command_pool, &internal_render.vertex_buffer, &internal_render.vertex_staging_buffer, sizeof(internal_render.vertices.data[0]) * internal_render.vertices.length);

        zz_internal_vulkan_buffer_load(&internal_render.index_staging_buffer, internal_render.indices.data, 0, sizeof(internal_render.indices.data[0]) * internal_render.indices.length, 0);
        zz_internal_vulkan_command_pool_copy_buffer(&internal_render.command_pool, &internal_render.index_buffer, &internal_render.index_staging_buffer, sizeof(internal_render.indices.data[0]) * internal_render.indices.length);

        zz_internal_vulkan_buffer_load(&internal_render.uniform_buffers.data[internal_render.current_frame], &internal_render.uniform_buffer_object, 0, sizeof(internal_render.uniform_buffer_object), 0);

        vkResetCommandBuffer(internal_render.command_pool.commandBuffers.data[internal_render.current_frame], 0);
        zz_internal_vulkan_record_command_buffer(&internal_render.vertex_buffer, &internal_render.index_buffer, internal_render.indices.length, &internal_render.pipeline, internal_render.current_frame, internal_render.command_pool.commandBuffers.data[internal_render.current_frame], internal_render.render_pass.renderPass, internal_render.pipeline.pipeline, &internal_render.swapchain.framebuffers, &internal_render.swapchain.extent, imageIndex);

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = ZZ_NULL;
        VkSemaphore submitWaitSemaphores[1] = {internal_render.sync.imageAvailableSemaphores.data[internal_render.current_frame]};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = submitWaitSemaphores;
        VkPipelineStageFlags submitWaitDstStageMask[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitDstStageMask = submitWaitDstStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &internal_render.command_pool.commandBuffers.data[internal_render.current_frame];
        VkSemaphore submitSignalSemaphores[1] = {internal_render.sync.renderFinishedSemaphores.data[internal_render.current_frame]};
        submitInfo.signalSemaphoreCount = 1; 
        submitInfo.pSignalSemaphores = submitSignalSemaphores;

        if (vkQueueSubmit(internal_render.device.graphicsQueue, 1, &submitInfo, internal_render.sync.inFlightFences.data[internal_render.current_frame]) != VK_SUCCESS)
        {
            return ZZ_FALSE;
        }

        VkPresentInfoKHR presentInfo;
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = ZZ_NULL;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = submitSignalSemaphores;
        VkSwapchainKHR swapchains[1] = {internal_render.swapchain.swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        uint32_t imageIndices[1] = {imageIndex};
        presentInfo.pImageIndices = imageIndices;
        presentInfo.pResults = ZZ_NULL;

        vkQueuePresentKHR(internal_render.device.presentQueue, &presentInfo);

        internal_render.current_frame = (internal_render.current_frame + 1) % internal_render.sync.max_frames_in_flight;

        zz_memory_array_clear(&internal_render.vertices);
        zz_memory_array_clear(&internal_render.indices);
    }

    return ZZ_TRUE;
}

void zz_internal_render_set_model_matrix(mat4 matrix)
{
    internal_render.uniform_buffer_object.model = matrix;
}

void zz_internal_render_set_view_matrix(mat4 matrix)
{
    internal_render.uniform_buffer_object.view = matrix;
}

void zz_internal_render_set_projection_matrix(mat4 matrix)
{
    internal_render.uniform_buffer_object.projection = matrix;
}

void zz_internal_render_draw_sprite(struct zz_sprite* sprite, vec3 position)
{
    u32 index = (u32)internal_render.vertices.length;

    zz_memory_array_push(&internal_render.vertices, &((struct zz_internal_vulkan_vertex){(vec3){position.x, position.y, position.z}, (vec2){0.0f, 0.0f}}));
    zz_memory_array_push(&internal_render.vertices, &((struct zz_internal_vulkan_vertex){(vec3){position.x + sprite->size.x, position.y, position.z}, (vec2){1.0f, 0.0f}}));
    zz_memory_array_push(&internal_render.vertices, &((struct zz_internal_vulkan_vertex){(vec3){position.x + sprite->size.x, position.y + sprite->size.y, position.z}, (vec2){1.0f, 1.0f}}));
    zz_memory_array_push(&internal_render.vertices, &((struct zz_internal_vulkan_vertex){(vec3){position.x, position.y + sprite->size.y, position.z}, (vec2){0.0f, 1.0f}}));

    zz_memory_array_push(&internal_render.indices, &index); ++index;
    zz_memory_array_push(&internal_render.indices, &index); ++index;
    zz_memory_array_push(&internal_render.indices, &index);
    zz_memory_array_push(&internal_render.indices, &index); ++index;
    zz_memory_array_push(&internal_render.indices, &index); index -= 3;
    zz_memory_array_push(&internal_render.indices, &index);
}

#endif