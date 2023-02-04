#if defined(ZZ_BACKEND_VULKAN)

#include "backend/render.h"
#include "render.h"

#include "zz/log.h"
#include "zz/application.h"

b8 backend_render_create(struct backend_render* backend_render, struct backend_render_config* config)
{
    backend_render->framebuffer_resized = FALSE;
    backend_render->current_frame = 0;

    backend_render->memory = config->memory;
    backend_render->application = config->application;

    backend_render->camera = NULL;

    memory_array_create_and_reserve(backend_render->memory, &backend_render->vertices, 1024);
    memory_array_create_and_reserve(backend_render->memory, &backend_render->indices, 1024);

    struct backend_vulkan_instance_config instance_config;
    if (!backend_vulkan_instance_create(&backend_render->instance, &instance_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan instance.");
        return FALSE;
    }

    struct backend_vulkan_surface_config surface_config;
    surface_config.application = backend_render->application;
    surface_config.instance = &backend_render->instance;
    if (!backend_vulkan_surface_create(&backend_render->surface, &surface_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan surface.");
        return FALSE;
    }

    struct backend_vulkan_device_config device_config;
    device_config.instance = &backend_render->instance;
    device_config.surface = &backend_render->surface;
    if (!backend_vulkan_device_create(&backend_render->device, &device_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan device.");
        return FALSE;
    }

    struct backend_vulkan_render_pass_config render_pass_config;
    render_pass_config.device = &backend_render->device;
    if (!backend_vulkan_render_pass_create(&backend_render->render_pass, &render_pass_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan render pass.");
        return FALSE;
    }

    struct backend_vulkan_swapchain_config swapchain_config;
    swapchain_config.memory = backend_render->memory;
    swapchain_config.application = backend_render->application;
    swapchain_config.surface = &backend_render->surface;
    swapchain_config.device = &backend_render->device;
    swapchain_config.render_pass = &backend_render->render_pass;
    if (!backend_vulkan_swapchain_create(&backend_render->swapchain, &swapchain_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan swapchain.");
        return FALSE;
    }
    
    struct backend_vulkan_sync_config sync_config;
    sync_config.memory = backend_render->memory;
    sync_config.device = &backend_render->device;
    sync_config.max_frames_in_flight = 2;
    if (!backend_vulkan_sync_create(&backend_render->sync, &sync_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan sync.");
        return FALSE;
    }

    struct backend_vulkan_buffer_config vertex_buffer_config;
    vertex_buffer_config.memory = backend_render->memory;
    vertex_buffer_config.device = &backend_render->device;
    vertex_buffer_config.usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertex_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    vertex_buffer_config.size = sizeof(struct backend_vulkan_vertex) * 1024;
    if (!backend_vulkan_buffer_create(&backend_render->vertex_buffer, &vertex_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return FALSE;
    }

    struct backend_vulkan_buffer_config vertex_staging_buffer_config;
    vertex_staging_buffer_config.memory = backend_render->memory;
    vertex_staging_buffer_config.device = &backend_render->device;
    vertex_staging_buffer_config.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    vertex_staging_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    vertex_staging_buffer_config.size = sizeof(struct backend_vulkan_vertex) * 1024;
    if (!backend_vulkan_buffer_create(&backend_render->vertex_staging_buffer, &vertex_staging_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return FALSE;
    }
    
    struct backend_vulkan_buffer_config index_buffer_config;
    index_buffer_config.memory = backend_render->memory;
    index_buffer_config.device = &backend_render->device;
    index_buffer_config.usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    index_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    index_buffer_config.size = sizeof(u32) * 1024;
    if (!backend_vulkan_buffer_create(&backend_render->index_buffer, &index_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return FALSE;
    }
    
    struct backend_vulkan_buffer_config index_staging_buffer_config;
    index_staging_buffer_config.memory = backend_render->memory;
    index_staging_buffer_config.device = &backend_render->device;
    index_staging_buffer_config.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    index_staging_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    index_staging_buffer_config.size = sizeof(u32) * 1024;
    if (!backend_vulkan_buffer_create(&backend_render->index_staging_buffer, &index_staging_buffer_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan vertex buffer.");
        return FALSE;
    }

    memory_array_create_and_reserve(backend_render->memory, &backend_render->uniform_buffers, backend_render->sync.max_frames_in_flight);
    for (u16 i = 0; i < backend_render->uniform_buffers.capacity; i += 1)
    {
        struct backend_vulkan_buffer_config uniform_buffer_config;
        uniform_buffer_config.memory = backend_render->memory;
        uniform_buffer_config.device = &backend_render->device;
        uniform_buffer_config.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        uniform_buffer_config.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        uniform_buffer_config.size = sizeof(struct backend_vulkan_uniform_buffer_object);
        if (!backend_vulkan_buffer_create(&backend_render->uniform_buffers.data[i], &uniform_buffer_config)) // TODO Implement persistent mapping
        {
            ZZ_LOG_FATAL("Failed to create Vulkan uniform buffer.");
            return FALSE;
        }
    }
    backend_render->uniform_buffers.length = backend_render->uniform_buffers.capacity;

    struct backend_vulkan_pipeline_config pipeline_config;
    pipeline_config.memory = backend_render->memory;
    pipeline_config.device = &backend_render->device;
    pipeline_config.render_pass = &backend_render->render_pass;
    pipeline_config.swapchain = &backend_render->swapchain;
    pipeline_config.uniform_buffers = &backend_render->uniform_buffers;
    if (!backend_vulkan_pipeline_create(&backend_render->pipeline, &pipeline_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan pipeline.");
        return FALSE;
    }

    struct backend_vulkan_command_pool_config command_pool_config;
    command_pool_config.memory = backend_render->memory;
    command_pool_config.device = &backend_render->device;
    command_pool_config.sync = &backend_render->sync;
    if (!backend_vulkan_command_pool_create(&backend_render->command_pool, &command_pool_config))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan command pool.");
        return FALSE;
    }

    return TRUE;
}

void backend_render_destroy(struct backend_render* backend_render)
{
    vkDeviceWaitIdle(backend_render->device.device);

    backend_vulkan_command_pool_destroy(&backend_render->command_pool);
    backend_vulkan_pipeline_destroy(&backend_render->pipeline);
    for (u16 i = 0; i < backend_render->uniform_buffers.length; i += 1)
    {
        backend_vulkan_buffer_destroy(&backend_render->uniform_buffers.data[i]);
    }
    memory_array_destroy(backend_render->memory, &backend_render->uniform_buffers);
    backend_vulkan_buffer_destroy(&backend_render->index_staging_buffer);
    backend_vulkan_buffer_destroy(&backend_render->index_buffer);
    backend_vulkan_buffer_destroy(&backend_render->vertex_staging_buffer);
    backend_vulkan_buffer_destroy(&backend_render->vertex_buffer);
    backend_vulkan_sync_destroy(&backend_render->sync);
    backend_vulkan_swapchain_destroy(&backend_render->swapchain);
    backend_vulkan_render_pass_destroy(&backend_render->render_pass);
    backend_vulkan_device_destroy(&backend_render->device);
    backend_vulkan_surface_destroy(&backend_render->surface);
    backend_vulkan_instance_destroy(&backend_render->instance);

    memory_array_destroy(backend_render->memory, &backend_render->indices);
    memory_array_destroy(backend_render->memory, &backend_render->vertices);
}

void backend_render_resize(struct backend_render* backend_render)
{
    u16 width, height;
    application_get_size(backend_render->application, &width, &height);
    if ((uint32_t)width != backend_render->swapchain.extent.width || (uint32_t)height != backend_render->swapchain.extent.height)
    {
        backend_render->framebuffer_resized = TRUE;
    }
}

b8 backend_render_draw_frame(struct backend_render* backend_render)
{
    if (backend_render->framebuffer_resized)
    {
        ZZ_LOG_INFO("Recreating Vulkan swapchain.");
        backend_vulkan_swapchain_recreate(&backend_render->swapchain);
        backend_render->framebuffer_resized = FALSE;
        return FALSE; 
    }

    if (backend_render->camera == NULL || backend_render->camera->position.z == 1.0f)
    {
        ZZ_LOG_INFO("Camera invalid.");
        return FALSE;
    }

    vkWaitForFences(backend_render->device.device, 1, &backend_render->sync.inFlightFences.data[backend_render->current_frame], VK_TRUE, (uint64_t)-1);

    uint32_t imageIndex;
    VkResult acquireNextImageResult = vkAcquireNextImageKHR(backend_render->device.device, backend_render->swapchain.swapchain, (uint64_t)-1, backend_render->sync.imageAvailableSemaphores.data[backend_render->current_frame], VK_NULL_HANDLE, &imageIndex);
    if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR)
    {
        ZZ_LOG_INFO("Recreating Vulkan swapchain.");
        backend_vulkan_swapchain_recreate(&backend_render->swapchain);
        return FALSE;
    }
    else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR)
    {
        return FALSE;
    }
    
    vkResetFences(backend_render->device.device, 1, &backend_render->sync.inFlightFences.data[backend_render->current_frame]);

    backend_vulkan_buffer_load(&backend_render->vertex_staging_buffer, backend_render->vertices.data, 0, sizeof(backend_render->vertices.data[0]) * backend_render->vertices.length, 0);
    backend_vulkan_command_pool_copy_buffer(&backend_render->command_pool, &backend_render->vertex_buffer, &backend_render->vertex_staging_buffer, sizeof(backend_render->vertices.data[0]) * backend_render->vertices.length);

    backend_vulkan_buffer_load(&backend_render->index_staging_buffer, backend_render->indices.data, 0, sizeof(backend_render->indices.data[0]) * backend_render->indices.length, 0);
    backend_vulkan_command_pool_copy_buffer(&backend_render->command_pool, &backend_render->index_buffer, &backend_render->index_staging_buffer, sizeof(backend_render->indices.data[0]) * backend_render->indices.length);

    struct backend_vulkan_uniform_buffer_object uniform_buffer_object;
    uniform_buffer_object.model = mat4_identity();
    uniform_buffer_object.view = camera_get_view_matrix(backend_render->camera);
    uniform_buffer_object.projection = camera_get_projection_matrix(backend_render->camera);
    backend_vulkan_buffer_load(&backend_render->uniform_buffers.data[backend_render->current_frame], &uniform_buffer_object, 0, sizeof(uniform_buffer_object), 0);

    vkResetCommandBuffer(backend_render->command_pool.commandBuffers.data[backend_render->current_frame], 0);
    backend_vulkan_record_command_buffer(&backend_render->vertex_buffer, &backend_render->index_buffer, backend_render->indices.length, &backend_render->pipeline, backend_render->current_frame, backend_render->command_pool.commandBuffers.data[backend_render->current_frame], backend_render->render_pass.renderPass, backend_render->pipeline.pipeline, &backend_render->swapchain.framebuffers, &backend_render->swapchain.extent, imageIndex);

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    VkSemaphore submitWaitSemaphores[1] = {backend_render->sync.imageAvailableSemaphores.data[backend_render->current_frame]};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = submitWaitSemaphores;
    VkPipelineStageFlags submitWaitDstStageMask[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = submitWaitDstStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &backend_render->command_pool.commandBuffers.data[backend_render->current_frame];
    VkSemaphore submitSignalSemaphores[1] = {backend_render->sync.renderFinishedSemaphores.data[backend_render->current_frame]};
    submitInfo.signalSemaphoreCount = 1; 
    submitInfo.pSignalSemaphores = submitSignalSemaphores;

    if (vkQueueSubmit(backend_render->device.graphicsQueue, 1, &submitInfo, backend_render->sync.inFlightFences.data[backend_render->current_frame]) != VK_SUCCESS)
    {
        return FALSE;
    }

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = NULL;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = submitSignalSemaphores;
    VkSwapchainKHR swapchains[1] = {backend_render->swapchain.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    uint32_t imageIndices[1] = {imageIndex};
    presentInfo.pImageIndices = imageIndices;
    presentInfo.pResults = NULL;

    vkQueuePresentKHR(backend_render->device.presentQueue, &presentInfo);

    backend_render->current_frame = (backend_render->current_frame + 1) % backend_render->sync.max_frames_in_flight;

    memory_array_clear(backend_render->memory, &backend_render->vertices);
    memory_array_clear(backend_render->memory, &backend_render->indices);

    return TRUE;
}

void backend_render_bind_camera(struct backend_render* backend_render, struct camera* camera)
{
    backend_render->camera = camera;
}

void backend_render_draw_sprite(struct backend_render* backend_render, struct sprite* sprite, vec3 position)
{
    u32 index = (u32)backend_render->vertices.length;

    memory_array_push(backend_render->memory, &backend_render->vertices, ((struct backend_vulkan_vertex){(vec3){position.x, position.y, position.z}}));
    memory_array_push(backend_render->memory, &backend_render->vertices, ((struct backend_vulkan_vertex){(vec3){position.x + sprite->size.x, position.y, position.z}}));
    memory_array_push(backend_render->memory, &backend_render->vertices, ((struct backend_vulkan_vertex){(vec3){position.x + sprite->size.x, position.y + sprite->size.y, position.z}}));
    memory_array_push(backend_render->memory, &backend_render->vertices, ((struct backend_vulkan_vertex){(vec3){position.x, position.y + sprite->size.y, position.z}}));

    memory_array_push(backend_render->memory, &backend_render->indices, index + 0);
    memory_array_push(backend_render->memory, &backend_render->indices, index + 1);
    memory_array_push(backend_render->memory, &backend_render->indices, index + 2);
    memory_array_push(backend_render->memory, &backend_render->indices, index + 2);
    memory_array_push(backend_render->memory, &backend_render->indices, index + 3);
    memory_array_push(backend_render->memory, &backend_render->indices, index + 0);
}

#endif