#include "backend.h"
#include "backend_vulkan.h"

#if defined(ZZ_BACKEND_VULKAN)

#include "utility/dynamic_array/dynamic_array.h"
#include "core/string/string.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/render/render.h"
#include "platform/backend/backend_vulkan.h"

static struct backend_vulkan_context backend_vulkan_context;
static u32 cached_framebuffer_width = 0;
static u32 cached_framebuffer_height = 0;

b8 backend_initialize(const char* application_name, struct platform_application* platform_application)
{
    // TODO Custom allocator
    backend_vulkan_context.allocator = 0;

    render_get_size(&cached_framebuffer_width, &cached_framebuffer_height);
    backend_vulkan_context.framebuffer_width = (cached_framebuffer_width != 0) ? cached_framebuffer_width : 800;
    backend_vulkan_context.framebuffer_height = (cached_framebuffer_height != 0) ? cached_framebuffer_height : 600;
    cached_framebuffer_width = 0;
    cached_framebuffer_height = 0;

    VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    applicationInfo.pApplicationName = application_name;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "Sleepy Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

#if defined(ZZ_DEBUG)
    const char** required_extension_names = dynamic_array_create(const char*);
    dynamic_array_push(required_extension_names, &VK_KHR_SURFACE_EXTENSION_NAME);
    dynamic_array_push(required_extension_names, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    platform_backend_vulkan_dynamic_array_push_required_extension_names(&required_extension_names);
    u32 required_extension_count = dynamic_array_get_length(required_extension_names);

    const char** required_layer_names = dynamic_array_create(const char*);
    dynamic_array_push(required_layer_names, &"VK_LAYER_KHRONOS_validation");
    u32 required_layer_count = dynamic_array_get_length(required_layer_names);
    ZZ_LOG_DEBUG("Required Vulkan layers:");

    u32 available_layer_count = 0;
    ZZ_BACKEND_VULKAN_ASSERT(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties* availableLayers = dynamic_array_create_and_reserve(VkLayerProperties, available_layer_count);
    ZZ_BACKEND_VULKAN_ASSERT(vkEnumerateInstanceLayerProperties(&available_layer_count, availableLayers));

    for (u32 i = 0; i < required_layer_count; i += 1)
    {
        b8 available = FALSE;
        for (u32 j = 0; j < available_layer_count; j += 1)
        {
            if (string_equals(required_layer_names[i], availableLayers[j].layerName))
            {
                available = TRUE;
                break;
            }
        }
        if (!available)
        {
            ZZ_LOG_FATAL("Required Vulkan layer %s unavailable.", required_layer_names[i]);
            return FALSE;
        }
    }
#elif defined(ZZ_RELEASE)
    const char** required_extension_names = dynamic_array_create(const char*);
    dynamic_array_push(required_extension_names, &VK_KHR_SURFACE_EXTENSION_NAME);
    platform_backend_vulkan_dynamic_array_push_required_extension_names(&required_extension_names);
    u32 required_extension_count = dynamic_array_get_length(required_extension_names);

    const char** required_layer_names = 0;
    u32 required_layer_count = 0;
#endif

    VkInstanceCreateInfo instanceCreateInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = required_extension_count;
    instanceCreateInfo.ppEnabledExtensionNames = required_extension_names;
    instanceCreateInfo.enabledLayerCount = required_layer_count;
    instanceCreateInfo.ppEnabledLayerNames = required_layer_names;

    ZZ_BACKEND_VULKAN_ASSERT(vkCreateInstance(&instanceCreateInfo, backend_vulkan_context.allocator, &backend_vulkan_context.instance));

#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugUtilsMessengerCreateInfo.pfnUserCallback = backend_vulkan_debug_callback;
    
    PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(backend_vulkan_context.instance, "vkCreateDebugUtilsMessengerEXT");
    ZZ_ASSERT_MESSAGE(createDebugUtilsMessengerEXT, "Failed to create VkDebugUtilsMessenger.");
    ZZ_BACKEND_VULKAN_ASSERT(createDebugUtilsMessengerEXT(backend_vulkan_context.instance, &debugUtilsMessengerCreateInfo, backend_vulkan_context.allocator, &backend_vulkan_context.debugUtilsMessenger));
#elif defined(ZZ_RELEASE)
#endif

    if (!platform_backend_vulkan_context_create_surface(platform_application, &backend_vulkan_context))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan surface.");
        return FALSE;
    }

    if (!backend_vulkan_context_create_device(&backend_vulkan_context))
    {
        ZZ_LOG_FATAL("Failed to create backend_vulkan_device.");
        return FALSE;
    }

    if (!backend_vulkan_swapchain_create(&backend_vulkan_context, backend_vulkan_context.framebuffer_width, backend_vulkan_context.framebuffer_height, &backend_vulkan_context.swapchain))
    {
        ZZ_LOG_FATAL("Failed to create backend_vulkan_swapchain.");
        return FALSE;
    }

    if (!backend_vulkan_render_pass_create(&backend_vulkan_context, &backend_vulkan_context.main_render_pass, 0, 0, backend_vulkan_context.framebuffer_width, backend_vulkan_context.framebuffer_height, 0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0))
    {
        ZZ_LOG_FATAL("Failed to create backend_vulkan_render_pass.");
        return FALSE;
    }

    backend_vulkan_context_generate_framebuffers(&backend_vulkan_context);

    backend_vulkan_context_generate_command_buffers(&backend_vulkan_context);

    backend_vulkan_context.imageAvailableSemaphores = dynamic_array_create_and_reserve(VkSemaphore, backend_vulkan_context.swapchain.max_frames_in_flight);
    backend_vulkan_context.queueCompleteSemaphores = dynamic_array_create_and_reserve(VkSemaphore, backend_vulkan_context.swapchain.max_frames_in_flight);
    backend_vulkan_context.in_flight_fences = dynamic_array_create_and_reserve(struct backend_vulkan_fence, backend_vulkan_context.swapchain.max_frames_in_flight);
    for (u32 i = 0; i < backend_vulkan_context.swapchain.max_frames_in_flight; i += 1)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        ZZ_BACKEND_VULKAN_ASSERT(vkCreateSemaphore(backend_vulkan_context.device.logicalDevice, &semaphoreCreateInfo, backend_vulkan_context.allocator, &backend_vulkan_context.imageAvailableSemaphores[i]));
        ZZ_BACKEND_VULKAN_ASSERT(vkCreateSemaphore(backend_vulkan_context.device.logicalDevice, &semaphoreCreateInfo, backend_vulkan_context.allocator, &backend_vulkan_context.queueCompleteSemaphores[i]));
    
        if (!backend_vulkan_fence_create(&backend_vulkan_context, &backend_vulkan_context.in_flight_fences[i], TRUE))
        {
            ZZ_LOG_FATAL("Failed to create backend_vulkan_fence.");
            return FALSE;
        }
    }
    backend_vulkan_context.images_fences_in_flight = dynamic_array_create_and_reserve(struct backend_vulkan_fence*, backend_vulkan_context.swapchain.image_count);
    for (u32 i = 0; i < backend_vulkan_context.swapchain.image_count; i += 1)
    {
        backend_vulkan_context.images_fences_in_flight = 0;
    }

    return TRUE;
}

void backend_deinitialize()
{
    vkDeviceWaitIdle(backend_vulkan_context.device.logicalDevice);

    for (u32 i = 0; i < backend_vulkan_context.swapchain.max_frames_in_flight; i += 1)
    {
        if (backend_vulkan_context.imageAvailableSemaphores[i])
        {
            vkDestroySemaphore(backend_vulkan_context.device.logicalDevice, backend_vulkan_context.imageAvailableSemaphores[i], backend_vulkan_context.allocator);
            backend_vulkan_context.imageAvailableSemaphores[i] = 0;
        }
        if (backend_vulkan_context.queueCompleteSemaphores[i])
        {
            vkDestroySemaphore(backend_vulkan_context.device.logicalDevice, backend_vulkan_context.queueCompleteSemaphores[i], backend_vulkan_context.allocator);
            backend_vulkan_context.queueCompleteSemaphores[i] = 0;
        }
        backend_vulkan_fence_destroy(&backend_vulkan_context, &backend_vulkan_context.in_flight_fences[i]);
    }
    dynamic_array_destroy(backend_vulkan_context.imageAvailableSemaphores);
    backend_vulkan_context.imageAvailableSemaphores = 0;
    dynamic_array_destroy(backend_vulkan_context.queueCompleteSemaphores);
    backend_vulkan_context.queueCompleteSemaphores = 0;
    dynamic_array_destroy(backend_vulkan_context.in_flight_fences);
    backend_vulkan_context.in_flight_fences = 0;

    backend_vulkan_render_pass_destroy(&backend_vulkan_context, &backend_vulkan_context.main_render_pass);
    backend_vulkan_swapchain_destroy(&backend_vulkan_context, &backend_vulkan_context.swapchain);
    backend_vulkan_context_destroy_device(&backend_vulkan_context);
    vkDestroySurfaceKHR(backend_vulkan_context.instance, backend_vulkan_context.surface, backend_vulkan_context.allocator);
    backend_vulkan_context.surface = 0;

#if defined(ZZ_DEBUG)
    if (backend_vulkan_context.debugUtilsMessenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(backend_vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyDebugUtilsMessengerEXT(backend_vulkan_context.instance, backend_vulkan_context.debugUtilsMessenger, backend_vulkan_context.allocator);
    }
#elif defined(ZZ_RELEASE)
#endif

    vkDestroyInstance(backend_vulkan_context.instance, backend_vulkan_context.allocator);
}

void backend_resize(u16 width, u16 height)
{

}

b8 backend_begin_frame(f32 delta_time)
{
    return TRUE;
}

b8 backend_end_frame(f32 delta_time)
{
    return TRUE;
}

void backend_vulkan_physical_device_query_swapchain_support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info)
{
    ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapchain_support_info->capabilities));

    ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &swapchain_support_info->surface_format_count, 0));
    if (swapchain_support_info->surface_format_count != 0)
    {
        if (!swapchain_support_info->surfaceFormats)
        {
            swapchain_support_info->surfaceFormats = memory_allocate(sizeof(VkSurfaceFormatKHR) * swapchain_support_info->surface_format_count, ZZ_MEMORY_TAG_RENDER);
        }
        ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &swapchain_support_info->surface_format_count, swapchain_support_info->surfaceFormats));
    }

    ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &swapchain_support_info->present_mode_count, 0));
    if (swapchain_support_info->surface_format_count != 0)
    {
        if (!swapchain_support_info->presentModes)
        {
            swapchain_support_info->presentModes = memory_allocate(sizeof(VkPresentModeKHR) * swapchain_support_info->present_mode_count, ZZ_MEMORY_TAG_RENDER);
        }
        ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &swapchain_support_info->present_mode_count, swapchain_support_info->presentModes));
    }
}

b8 backend_vulkan_physical_device_meets_requirements(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkPhysicalDeviceProperties* properties, const VkPhysicalDeviceFeatures* features, const struct backend_vulkan_physical_device_requirements* requirements, struct backend_vulkan_physical_device_queue_family_info* queue_family_info, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info)
{
    queue_family_info->graphics_index = -1;
    queue_family_info->present_index = -1;
    queue_family_info->compute_index = -1;
    queue_family_info->transfer_index = -1;

    if (requirements->discrete_gpu)
    {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return FALSE;
        }
    }

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_family_count, 0);
    VkQueueFamilyProperties queueFamilies[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_family_count, queueFamilies);

    u8 min_transfer_score = -1;
    for (u32 i = 0; i < queue_family_count; i += 1)
    {
        u8 current_transfer_score = 0;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queue_family_info->graphics_index = i;
            current_transfer_score += 1;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            queue_family_info->compute_index = i;
            current_transfer_score += 1;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            if (current_transfer_score <= min_transfer_score)
            {
                queue_family_info->transfer_index = i;
                min_transfer_score = current_transfer_score;
            }
        }

        VkBool32 supportsPresent = VK_FALSE;
        ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportsPresent));
        if (supportsPresent)
        {
            queue_family_info->present_index = i;
        }
    }

    if (requirements->graphics && queue_family_info->graphics_index == -1)
    {
        return FALSE;
    }
    if (requirements->present && queue_family_info->present_index == -1)
    {
        return FALSE;
    }
    if (requirements->compute && queue_family_info->compute_index == -1)
    {
        return FALSE;
    }
    if (requirements->transfer && queue_family_info->transfer_index == -1)
    {
        return FALSE;
    }
    
    if (requirements->sampler_anisotropy && !features->samplerAnisotropy)
    {
        return FALSE;
    }

    backend_vulkan_physical_device_query_swapchain_support(physicalDevice, surface, swapchain_support_info);
    if (swapchain_support_info->surface_format_count == 0 || swapchain_support_info->present_mode_count == 0)
    {
        if (swapchain_support_info->surfaceFormats)
        {
            memory_deallocate(swapchain_support_info->surfaceFormats, sizeof(VkSurfaceFormatKHR) * swapchain_support_info->surface_format_count, ZZ_MEMORY_TAG_RENDER);
        }
        if (swapchain_support_info->presentModes)
        {
            memory_deallocate(swapchain_support_info->presentModes, sizeof(VkPresentModeKHR) * swapchain_support_info->present_mode_count, ZZ_MEMORY_TAG_RENDER);
        }
        return FALSE;
    }

    if (requirements->extension_names)
    {
        u32 available_extension_count = 0;
        VkExtensionProperties* availableExtensions = 0;
        ZZ_BACKEND_VULKAN_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &available_extension_count, 0));
        if (available_extension_count != 0)
        {
            availableExtensions = memory_allocate(sizeof(VkExtensionProperties) * available_extension_count, ZZ_MEMORY_TAG_RENDER);
            ZZ_BACKEND_VULKAN_ASSERT(vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &available_extension_count, availableExtensions));

            u32 required_extension_count = dynamic_array_get_length(requirements->extension_names);
            for (u32 i = 0; i < required_extension_count; i += 1)
            {
                b8 available = FALSE;
                for (u32 j = 0; j < available_extension_count; j += 1)
                {
                    if (string_equals(requirements->extension_names[i], availableExtensions[j].extensionName))
                    {
                        available = TRUE;
                        break;
                    }
                }
                if (!available)
                {
                    return FALSE;
                }
            }

            memory_deallocate(availableExtensions, sizeof(VkExtensionProperties) * available_extension_count, ZZ_MEMORY_TAG_RENDER);
        }
    }

    return TRUE;
}

b8 backend_vulkan_context_select_physical_device(struct backend_vulkan_context* context)
{
    u32 physical_device_count = 0;
    ZZ_BACKEND_VULKAN_ASSERT(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, 0));
    if (physical_device_count == 0)
    {
        ZZ_LOG_FATAL("No suitable physical devices found.");
        return FALSE;
    }
    VkPhysicalDevice physicalDevices[physical_device_count];
    ZZ_BACKEND_VULKAN_ASSERT(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physicalDevices));

    for (u32 i = 0; i < physical_device_count; i += 1)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &memoryProperties);

        // TODO Abstract hardcoded requirements
        struct backend_vulkan_physical_device_requirements requirements = {};
        requirements.graphics = TRUE;
        requirements.present = TRUE;
        //requirements.compute = TRUE;
        requirements.transfer = TRUE;
        //requirements.sampler_anisotropy = TRUE;
        requirements.discrete_gpu = TRUE;
        requirements.extension_names = dynamic_array_create(const char*);
        dynamic_array_push(requirements.extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        struct backend_vulkan_physical_device_queue_family_info queue_family_info = {};
        b8 meets_requirements = backend_vulkan_physical_device_meets_requirements(physicalDevices[i], context->surface, &properties, &features, &requirements, &queue_family_info, &context->device.swapchain_support_info);
        
        if (meets_requirements)
        {
            for (u32 j = 0; j < memoryProperties.memoryHeapCount; j += 1) {
                f32 memory_size_gib = (((f32)memoryProperties.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);
                if (memoryProperties.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    ZZ_LOG_INFO("Local GPU memory: %.2f GiB", memory_size_gib);
                } else {
                    ZZ_LOG_INFO("Shared System memory: %.2f GiB", memory_size_gib);
                }
            }
            
            context->device.physicalDevice = physicalDevices[i];
            context->device.physicalDeviceProperties = properties;
            context->device.physicalDeviceFeatures = features;
            context->device.physicalDeviceMemoryProperties = memoryProperties;
            context->device.queue_family_info.graphics_index = queue_family_info.graphics_index;
            context->device.queue_family_info.present_index = queue_family_info.present_index;
            context->device.queue_family_info.transfer_index = queue_family_info.transfer_index;
            context->device.queue_family_info.compute_index = queue_family_info.compute_index;

            break;
        }
    }

    if (!context->device.physicalDevice)
    {
        return FALSE;
    }

    return TRUE;
}

b8 backend_vulkan_context_create_device(struct backend_vulkan_context* context)
{
    if (!backend_vulkan_context_select_physical_device(context))
    {
        return FALSE;
    }

    // TODO Abstract hardcoded requirements
    b8 graphics_queue_family_shares_present = context->device.queue_family_info.graphics_index == context->device.queue_family_info.present_index;
    b8 graphics_queue_family_shares_compute = context->device.queue_family_info.graphics_index == context->device.queue_family_info.compute_index;
    b8 graphics_queue_family_shares_transfer = context->device.queue_family_info.graphics_index == context->device.queue_family_info.transfer_index;
    b8 present_queue_family_shares_compute = context->device.queue_family_info.present_index == context->device.queue_family_info.compute_index;
    b8 present_queue_family_shares_transfer = context->device.queue_family_info.present_index == context->device.queue_family_info.transfer_index;
    b8 compute_queue_family_shares_transfer = context->device.queue_family_info.compute_index == context->device.queue_family_info.transfer_index;
    u32 queue_family_count = 1;
    if (!graphics_queue_family_shares_present)
    {
        queue_family_count += 1;
    }
    if (!graphics_queue_family_shares_compute && !present_queue_family_shares_compute)
    {
        queue_family_count += 1;
    }
    if (!graphics_queue_family_shares_transfer && !present_queue_family_shares_transfer && !compute_queue_family_shares_transfer)
    {
        queue_family_count += 1;
    }
    u32 queue_family_indices[queue_family_count];
    u8 index = 0;
    queue_family_indices[index++] = context->device.queue_family_info.graphics_index;
    if (!graphics_queue_family_shares_present)
    {
        queue_family_indices[index++] = context->device.queue_family_info.present_index;
    }
    if (!graphics_queue_family_shares_compute && !present_queue_family_shares_compute)
    {
        queue_family_indices[index++] = context->device.queue_family_info.compute_index;
    }
    if (!graphics_queue_family_shares_transfer && !present_queue_family_shares_transfer && !compute_queue_family_shares_transfer)
    {
        queue_family_indices[index++] = context->device.queue_family_info.transfer_index;
    }

    VkDeviceQueueCreateInfo deviceQueueCreateInfos[queue_family_count];
    for (u32 i = 0; i < queue_family_count; i += 1)
    {
        deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfos[i].queueFamilyIndex = queue_family_indices[i];
        deviceQueueCreateInfos[i].queueCount = 1;
        deviceQueueCreateInfos[i].flags = 0;
        deviceQueueCreateInfos[i].pNext = 0;
        f32 queue_priority = 1.0f;
        deviceQueueCreateInfos[i].pQueuePriorities = &queue_priority;
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
    //physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

    // TODO Abstract hardcoded device creation
    VkDeviceCreateInfo deviceCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    deviceCreateInfo.queueCreateInfoCount = queue_family_count;
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 1;
    const char* extension_names[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    deviceCreateInfo.ppEnabledExtensionNames = extension_names;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = 0;
    ZZ_BACKEND_VULKAN_ASSERT(vkCreateDevice(context->device.physicalDevice, &deviceCreateInfo, context->allocator, &context->device.logicalDevice));

    vkGetDeviceQueue(context->device.logicalDevice, context->device.queue_family_info.graphics_index, 0, &context->device.graphicsQueue);
    vkGetDeviceQueue(context->device.logicalDevice, context->device.queue_family_info.present_index, 0, &context->device.presentQueue);
    vkGetDeviceQueue(context->device.logicalDevice, context->device.queue_family_info.compute_index, 0, &context->device.computeQueue);
    vkGetDeviceQueue(context->device.logicalDevice, context->device.queue_family_info.transfer_index, 0, &context->device.transferQueue);
    
    VkCommandPoolCreateInfo commandPoolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    commandPoolCreateInfo.queueFamilyIndex = context->device.queue_family_info.graphics_index;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    ZZ_BACKEND_VULKAN_ASSERT(vkCreateCommandPool(context->device.logicalDevice, &commandPoolCreateInfo, context->allocator, &context->device.graphicsCommandPool));

    return TRUE;
}

void backend_vulkan_context_destroy_device(struct backend_vulkan_context* context)
{
    context->device.graphicsQueue = 0;
    context->device.presentQueue = 0;
    context->device.computeQueue = 0;
    context->device.transferQueue = 0;

    context->device.queue_family_info.graphics_index = -1;
    context->device.queue_family_info.present_index = -1;
    context->device.queue_family_info.compute_index = -1;
    context->device.queue_family_info.transfer_index = -1;

    if (context->device.graphicsCommandPool)
    {
        vkDestroyCommandPool(context->device.logicalDevice, context->device.graphicsCommandPool, context->allocator);
        context->device.graphicsCommandPool = 0;
    }

    if (context->device.logicalDevice)
    {
        vkDestroyDevice(context->device.logicalDevice, context->allocator);
        context->device.logicalDevice = 0;
    }

    memory_zero(&context->device.swapchain_support_info.capabilities, sizeof(context->device.swapchain_support_info.capabilities));

    if (context->device.swapchain_support_info.surfaceFormats)
    {
        memory_deallocate(context->device.swapchain_support_info.surfaceFormats, sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support_info.surface_format_count, ZZ_MEMORY_TAG_RENDER);
        context->device.swapchain_support_info.surfaceFormats = 0;
        context->device.swapchain_support_info.surface_format_count = 0;
    }

    if (context->device.swapchain_support_info.presentModes)
    {
        memory_deallocate(context->device.swapchain_support_info.presentModes, sizeof(VkPresentModeKHR) * context->device.swapchain_support_info.present_mode_count, ZZ_MEMORY_TAG_RENDER);
        context->device.swapchain_support_info.presentModes = 0;
        context->device.swapchain_support_info.present_mode_count = 0;
    }

    context->device.physicalDevice = 0;
}

b8 backend_vulkan_device_detect_depth_format(struct backend_vulkan_device* device)
{
    const u64 format_candidate_count = 3;
    VkFormat formatCandidates[3] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};

    u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    for (u64 i = 0; i < format_candidate_count; i += 1)
    {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(device->physicalDevice, formatCandidates[i], &formatProperties);

        if ((formatProperties.linearTilingFeatures & flags) == flags || (formatProperties.optimalTilingFeatures & flags) == flags)
        {
            device->depthFormat = formatCandidates[i];
            return TRUE;
        }
    }

    return FALSE;
}

void backend_vulkan_image_create(struct backend_vulkan_context* context, VkImageType imageType, u32 width, u32 height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, b32 create_view, VkImageAspectFlags imageAspectFlags, struct backend_vulkan_image* image)
{
    image->width = width;
    image->height = height;

    VkImageCreateInfo imageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 4;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = format;
    imageCreateInfo.tiling = imageTiling;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = imageUsageFlags;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ZZ_BACKEND_VULKAN_ASSERT(vkCreateImage(context->device.logicalDevice, &imageCreateInfo, context->allocator, &image->handle));

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(context->device.logicalDevice, image->handle, &memoryRequirements);

    i32 memory_type_index = backend_vulkan_context_get_memory_index(context, memoryRequirements.memoryTypeBits, memoryPropertyFlags);
    if (memory_type_index == -1)
    {
        ZZ_LOG_ERROR("Required memory type not found.");
    }

    VkMemoryAllocateInfo memoryAllocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memory_type_index;
    ZZ_BACKEND_VULKAN_ASSERT(vkAllocateMemory(context->device.logicalDevice, &memoryAllocateInfo, context->allocator, &image->memory));
    ZZ_BACKEND_VULKAN_ASSERT(vkBindImageMemory(context->device.logicalDevice, image->handle, image->memory, 0));

    if (create_view)
    {
        image->view = 0;
        backend_vulkan_image_view_create(context, format, image, imageAspectFlags);
    }
}

void backend_vulkan_image_view_create(struct backend_vulkan_context* context, VkFormat format, struct backend_vulkan_image* image, VkImageAspectFlags imageAspectFlags)
{
    VkImageViewCreateInfo imageViewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    imageViewCreateInfo.image = image->handle;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = format;
    imageViewCreateInfo.subresourceRange.aspectMask = imageAspectFlags;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    ZZ_BACKEND_VULKAN_ASSERT(vkCreateImageView(context->device.logicalDevice, &imageViewCreateInfo, context->allocator, &image->view));
}

void backend_vulkan_image_destroy(struct backend_vulkan_context* context, struct backend_vulkan_image* image)
{
    if (image->view)
    {
        vkDestroyImageView(context->device.logicalDevice, image->view, context->allocator);
        image->view = 0;
    }

    if (image->memory)
    {
        vkFreeMemory(context->device.logicalDevice, image->memory, context->allocator);
        image->memory = 0;
    }

    if (image->handle)
    {
        vkDestroyImage(context->device.logicalDevice, image->handle, context->allocator);
        image->handle = 0;
    }
}

b8 backend_vulkan_render_pass_create(struct backend_vulkan_context* context, struct backend_vulkan_render_pass* render_pass, f32 x, f32 y, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a, f32 depth, u32 stencil)
{
    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    
    u32 attachment_description_count = 2;
    VkAttachmentDescription attachmentDescriptions[attachment_description_count];

    VkAttachmentDescription colorAttachmentDescription = {};
    colorAttachmentDescription.format = context->swapchain.imageSurfaceFormat.format;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachmentDescription.flags = 0;

    attachmentDescriptions[0] = colorAttachmentDescription;

    VkAttachmentReference colorAttachmentReference;
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;

    VkAttachmentDescription depthStencilAttachmentDescription = {};
    depthStencilAttachmentDescription.format = context->device.depthFormat;
    depthStencilAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    depthStencilAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthStencilAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthStencilAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthStencilAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthStencilAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthStencilAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachmentDescriptions[1] = depthStencilAttachmentDescription;

    VkAttachmentReference depthStencilAttachmentReference;
    depthStencilAttachmentReference.attachment = 1;
    depthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    subpassDescription.pDepthStencilAttachment = &depthStencilAttachmentReference;

    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = 0;

    subpassDescription.pResolveAttachments = 0;

    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = 0;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderPassCreateInfo.attachmentCount = attachment_description_count;
    renderPassCreateInfo.pAttachments = attachmentDescriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;
    renderPassCreateInfo.pNext = 0;
    renderPassCreateInfo.flags = 0;

    ZZ_BACKEND_VULKAN_ASSERT(vkCreateRenderPass(context->device.logicalDevice, &renderPassCreateInfo, context->allocator, &render_pass->handle));
    return TRUE;
}

void backend_vulkan_render_pass_destroy(struct backend_vulkan_context* context, struct backend_vulkan_render_pass* render_pass)
{
    if (render_pass && render_pass->handle)
    {
        vkDestroyRenderPass(context->device.logicalDevice, render_pass->handle, context->allocator);
        render_pass->handle = 0;
    }
}

void backend_vulkan_render_pass_begin(struct backend_vulkan_render_pass* render_pass, struct backend_vulkan_command_buffer* command_buffer, VkFramebuffer framebuffer)
{
    VkClearValue clearValues[2];
    memory_zero(clearValues, sizeof(VkClearValue) * 2);
    clearValues[0].color.float32[0] = render_pass->r;
    clearValues[0].color.float32[1] = render_pass->g;
    clearValues[0].color.float32[2] = render_pass->b;
    clearValues[0].color.float32[3] = render_pass->a;
    clearValues[1].depthStencil.depth = render_pass->depth;
    clearValues[1].depthStencil.stencil = render_pass->stencil;

    VkRenderPassBeginInfo renderPassBeginInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    renderPassBeginInfo.renderPass = render_pass->handle;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea.offset.x = render_pass->x;
    renderPassBeginInfo.renderArea.offset.y = render_pass->x;
    renderPassBeginInfo.renderArea.extent.width = render_pass->w;
    renderPassBeginInfo.renderArea.extent.height = render_pass->h;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(command_buffer->handle, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_IN_RENDER_PASS;
}

void backend_vulkan_render_pass_end(struct backend_vulkan_render_pass* render_pass, struct backend_vulkan_command_buffer* command_buffer)
{
    vkCmdEndRenderPass(command_buffer->handle);
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_RECORDING;
}

b8 backend_vulkan_framebuffer_create(struct backend_vulkan_context* context, struct backend_vulkan_framebuffer* framebuffer, struct backend_vulkan_render_pass* render_pass, u32 width, u32 height, u32 attachment_image_view_count, VkImageView* attachmentImageViews)
{
    framebuffer->attachment_image_view_count = attachment_image_view_count;
    framebuffer->attachmentImageViews = memory_allocate(sizeof(VkImageView) * attachment_image_view_count, ZZ_MEMORY_TAG_RENDER);
    for (u32 i = 0; i < attachment_image_view_count; i += 1)
    {
        framebuffer->attachmentImageViews[i] = attachmentImageViews[i];
    }

    framebuffer->render_pass = render_pass;

    VkFramebufferCreateInfo framebufferCreateInfo = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebufferCreateInfo.renderPass = render_pass->handle;
    framebufferCreateInfo.attachmentCount = attachment_image_view_count;
    framebufferCreateInfo.pAttachments = framebuffer->attachmentImageViews;
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = 1;
    ZZ_BACKEND_VULKAN_ASSERT(vkCreateFramebuffer(context->device.logicalDevice, &framebufferCreateInfo, context->allocator, &framebuffer->handle));

    return TRUE;
}

void backend_vulkan_framebuffer_destroy(struct backend_vulkan_context* context, struct backend_vulkan_framebuffer* framebuffer)
{
    vkDestroyFramebuffer(context->device.logicalDevice, framebuffer->handle, context->allocator);
    framebuffer->handle = 0;

    if (framebuffer->attachmentImageViews)
    {
        memory_deallocate(framebuffer->attachmentImageViews, sizeof(VkImageView) * framebuffer->attachment_image_view_count, ZZ_MEMORY_TAG_RENDER);
        framebuffer->attachmentImageViews = 0;
    }

    framebuffer->attachment_image_view_count = 0;
    framebuffer->render_pass = 0;
}

b8 backend_vulkan_swapchain_create(struct backend_vulkan_context* context, u32 width, u32 height, struct backend_vulkan_swapchain* swapchain)
{
    VkExtent2D swapchainExtent = {width, height};
    swapchain->max_frames_in_flight = 2;

    b8 preferenceAvailable = FALSE;
    for (u32 i = 0; i < context->device.swapchain_support_info.surface_format_count; i += 1)
    {
        VkSurfaceFormatKHR surfaceFormat = context->device.swapchain_support_info.surfaceFormats[i];

        if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            swapchain->imageSurfaceFormat = surfaceFormat;
            preferenceAvailable = TRUE;
            break;
        }
    }
    if (!preferenceAvailable)
    {
        swapchain->imageSurfaceFormat = context->device.swapchain_support_info.surfaceFormats[0];
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < context->device.swapchain_support_info.present_mode_count; i += 1)
    {
        VkPresentModeKHR mode = context->device.swapchain_support_info.presentModes[i];
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            presentMode = mode;
            break;
        }
    }

    backend_vulkan_physical_device_query_swapchain_support(context->device.physicalDevice, context->surface, &context->device.swapchain_support_info);
    if (context->device.swapchain_support_info.capabilities.currentExtent.width != (uint32_t)-1)
    {
        swapchainExtent = context->device.swapchain_support_info.capabilities.currentExtent;
    }

    VkExtent2D minExtent = context->device.swapchain_support_info.capabilities.minImageExtent;
    VkExtent2D maxExtent = context->device.swapchain_support_info.capabilities.maxImageExtent;
    swapchainExtent.width = ZZ_UTILITY_CLAMP(swapchainExtent.width, minExtent.width, maxExtent.width);
    swapchainExtent.height = ZZ_UTILITY_CLAMP(swapchainExtent.height, minExtent.height, maxExtent.height);

    u32 swapchain_image_count = context->device.swapchain_support_info.capabilities.minImageCount + 1;
    if (context->device.swapchain_support_info.capabilities.maxImageCount > 0 && swapchain_image_count > context->device.swapchain_support_info.capabilities.maxImageCount)
    {
        swapchain_image_count = context->device.swapchain_support_info.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swapchainCreateInfo.surface = context->surface;
    swapchainCreateInfo.minImageCount = swapchain_image_count;
    swapchainCreateInfo.imageFormat = swapchain->imageSurfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = swapchain->imageSurfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = swapchainExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (context->device.queue_family_info.graphics_index != context->device.queue_family_info.present_index)
    {
        u32 queue_family_indices[] = {(u32)context->device.queue_family_info.graphics_index, (u32)context->device.queue_family_info.present_index};
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = 0;
    }
    swapchainCreateInfo.preTransform = context->device.swapchain_support_info.capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = 0;

    ZZ_BACKEND_VULKAN_ASSERT(vkCreateSwapchainKHR(context->device.logicalDevice, &swapchainCreateInfo, context->allocator, &swapchain->handle));

    context->current_frame = 0;

    swapchain->image_count = 0;
    ZZ_BACKEND_VULKAN_ASSERT(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->image_count, 0));
    if (!swapchain->images)
    {
        swapchain->images = (VkImage*)memory_allocate(sizeof(VkImage) * swapchain->image_count, ZZ_MEMORY_TAG_RENDER);
    }
    if (!swapchain->imageViews)
    {
        swapchain->imageViews = (VkImageView*)memory_allocate(sizeof(VkImageView) * swapchain->image_count, ZZ_MEMORY_TAG_RENDER);
    }
    ZZ_BACKEND_VULKAN_ASSERT(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->image_count, swapchain->images));

    for (u32 i = 0; i < swapchain->image_count; i += 1)
    {
        VkImageViewCreateInfo imageViewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        imageViewCreateInfo.image = swapchain->images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = swapchain->imageSurfaceFormat.format;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        ZZ_BACKEND_VULKAN_ASSERT(vkCreateImageView(context->device.logicalDevice, &imageViewCreateInfo, context->allocator, &swapchain->imageViews[i]));
    }
    
    if (!backend_vulkan_device_detect_depth_format(&context->device))
    {
        context->device.depthFormat = VK_FORMAT_UNDEFINED;
        ZZ_LOG_FATAL("Failed to find a depth format.");
        return FALSE;
    }

    backend_vulkan_image_create(context, VK_IMAGE_TYPE_2D, swapchainExtent.width, swapchainExtent.height, context->device.depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TRUE, VK_IMAGE_ASPECT_DEPTH_BIT, &swapchain->depth_image);

    swapchain->framebuffers = dynamic_array_create_and_reserve(struct backend_vulkan_framebuffer, backend_vulkan_context.swapchain.image_count);

    return TRUE;
}

b8 backend_vulkan_swapchain_recreate(struct backend_vulkan_context* context, u32 width, u32 height, struct backend_vulkan_swapchain* swapchain)
{
    backend_vulkan_swapchain_destroy(context, swapchain);
    return backend_vulkan_swapchain_create(context, width, height, swapchain);
}

void backend_vulkan_swapchain_destroy(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain)
{
    for (u32 i = 0; i < swapchain->image_count; i += 1)
    {
        backend_vulkan_framebuffer_destroy(context, &swapchain->framebuffers[i]);
        vkDestroyImageView(context->device.logicalDevice, swapchain->imageViews[i], context->allocator);
    }

    backend_vulkan_image_destroy(context, &swapchain->depth_image);

    vkDestroySwapchainKHR(context->device.logicalDevice, swapchain->handle, context->allocator);
}

b8 backend_vulkan_swapchain_acquire_next_image_index(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain, u64 timeout_nanoseconds, VkSemaphore imageAvailableSemaphore, VkFence fence, u32* image_index)
{
    VkResult result = vkAcquireNextImageKHR(context->device.logicalDevice, swapchain->handle, timeout_nanoseconds, imageAvailableSemaphore, fence, image_index);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        backend_vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
        return FALSE;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        ZZ_LOG_FATAL("Failed to acquire swapchain image.");
        return FALSE;
    }

    return TRUE;
}

void backend_vulkan_swapchain_present(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, u32 present_image_index)
{
    VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain->handle;
    presentInfo.pImageIndices = &present_image_index;
    presentInfo.pResults = 0;

    VkResult result = vkQueuePresentKHR(context->device.presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        backend_vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
    }
    else if (result != VK_SUCCESS)
    {
        ZZ_LOG_FATAL("Failed to present swapchain image.");
    }

    context->current_frame = (context->current_frame + 1) % swapchain->max_frames_in_flight;
}

b8 backend_vulkan_command_buffer_allocate(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool, b8 primary)
{
    memory_zero(command_buffer, sizeof(command_buffer));
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_UNALLOCATED;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = 1;
    commandBufferAllocateInfo.pNext = 0;

    ZZ_BACKEND_VULKAN_ASSERT(vkAllocateCommandBuffers(context->device.logicalDevice, &commandBufferAllocateInfo, &command_buffer->handle));
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_READY;
    return TRUE;
}

void backend_vulkan_command_buffer_deallocate(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool)
{
    vkFreeCommandBuffers(context->device.logicalDevice, commandPool, 1, &command_buffer->handle);
    command_buffer->handle = 0;
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_UNALLOCATED;
}

void backend_vulkan_command_buffer_begin(struct backend_vulkan_command_buffer* command_buffer, b8 single_use, b8 render_pass_continue, b8 simultaneous_use)
{
    VkCommandBufferBeginInfo commandBufferBeginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    commandBufferBeginInfo.flags = 0;
    if (single_use)
    {
        commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (render_pass_continue)
    {
        commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (simultaneous_use)
    {
        commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    ZZ_BACKEND_VULKAN_ASSERT(vkBeginCommandBuffer(command_buffer->handle, &commandBufferBeginInfo));
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_RECORDING;
}

void backend_vulkan_command_buffer_end(struct backend_vulkan_command_buffer* command_buffer)
{
    ZZ_BACKEND_VULKAN_ASSERT(vkEndCommandBuffer(command_buffer->handle));
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_FINISHED;
}

void backend_vulkan_command_buffer_reset(struct backend_vulkan_command_buffer* command_buffer)
{
    command_buffer->state = ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_READY;
}

void backend_vulkan_command_buffer_allocate_and_begin_for_single_use(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool)
{
    backend_vulkan_command_buffer_allocate(context, command_buffer, commandPool, TRUE);
    backend_vulkan_command_buffer_begin(command_buffer, TRUE, FALSE, FALSE);
}

void backend_vulkan_command_buffer_end_and_submit_for_single_use(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool, VkQueue queue)
{
    backend_vulkan_command_buffer_end(command_buffer);

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command_buffer->handle;
    ZZ_BACKEND_VULKAN_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, 0));

    ZZ_BACKEND_VULKAN_ASSERT(vkQueueWaitIdle(queue));

    backend_vulkan_command_buffer_deallocate(context, command_buffer, commandPool);
}

b8 backend_vulkan_fence_create(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence, b8 signaled)
{
    fence->signaled = signaled;

    VkFenceCreateInfo fenceCreateInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceCreateInfo.flags = fence->signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    ZZ_BACKEND_VULKAN_ASSERT(vkCreateFence(context->device.logicalDevice, &fenceCreateInfo, context->allocator, &fence->handle));

    return TRUE;
}

void backend_vulkan_fence_destroy(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence)
{
    vkDestroyFence(context->device.logicalDevice, fence->handle, context->allocator);
    fence->handle = 0;
    fence->signaled = FALSE;
}

b8 backend_vulkan_fence_wait(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence, u64 timeout_nanoseconds)
{
    if (!fence->signaled)
    {
        VkResult result = vkWaitForFences(context->device.logicalDevice, 1, &fence->handle, TRUE, timeout_nanoseconds);
        switch(result)
        {
            case VK_SUCCESS:
                fence->signaled = TRUE;
                return TRUE;
            case VK_TIMEOUT:
                ZZ_LOG_WARNING("vkWaitForFences timed out.");
                break;
            case VK_ERROR_DEVICE_LOST:
                ZZ_LOG_ERROR("vkWaitForFences returned VK_ERROR_DEVICE_LOST.");
                break;
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                ZZ_LOG_ERROR("vkWaitForFences returned VK_ERROR_OUT_OF_HOST_MEMORY.");
                break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                ZZ_LOG_ERROR("vkWaitForFences returned VK_ERROR_OUT_OF_DEVICE_MEMORY.");
                break;
            default:
                ZZ_LOG_ERROR("vkWaitForFences returned an unknown error.");
                break;
        }
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

void backend_vulkan_fence_reset(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence)
{
    if (fence->signaled)
    {
        ZZ_BACKEND_VULKAN_ASSERT(vkResetFences(context->device.logicalDevice, 1, &fence->handle));
        fence->signaled = FALSE;
    }
}

void backend_vulkan_context_generate_framebuffers(struct backend_vulkan_context* context)
{
    for (u32 i = 0; i < context->swapchain.image_count; i += 1)
    {
        u32 attachment_image_view_count = 2;
        VkImageView attachmentImageViews[] = {context->swapchain.imageViews[i], context->swapchain.depth_image.view};

        backend_vulkan_framebuffer_create(context, &context->swapchain.framebuffers[i], &context->main_render_pass, context->framebuffer_width, context->framebuffer_height, attachment_image_view_count, attachmentImageViews);
    }
}

void backend_vulkan_context_generate_command_buffers(struct backend_vulkan_context* context)
{
    if (!context->graphics_command_buffers)
    {
        context->graphics_command_buffers = dynamic_array_create_and_reserve(struct backend_vulkan_command_buffer, context->swapchain.image_count);
        for (u32 i = 0; i < context->swapchain.image_count; i += 1)
        {
            memory_zero(&context->graphics_command_buffers[i], sizeof(struct backend_vulkan_command_buffer));
        }
    }

    for (u32 i = 0; i < context->swapchain.image_count; i += 1)
    {
        if (context->graphics_command_buffers[i].handle)
        {
            backend_vulkan_command_buffer_deallocate(context, &context->graphics_command_buffers[i], context->device.graphicsCommandPool);
        }
        memory_zero(&context->graphics_command_buffers[i], sizeof(struct backend_vulkan_command_buffer));
        backend_vulkan_command_buffer_allocate(context, &context->graphics_command_buffers[i], context->device.graphicsCommandPool, TRUE);
    }
}

i32 backend_vulkan_context_get_memory_index(struct backend_vulkan_context* context, u32 type_filter, u32 property_flags)
{
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(context->device.physicalDevice, &physicalDeviceMemoryProperties);

    for (u32 i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i += 1)
    {
        if (type_filter & (1 << i) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & property_flags) == property_flags)
        {
            return i;
        }
    }

    return -1;
}

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    switch (messageSeverity)
    {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ZZ_LOG_ERROR(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            ZZ_LOG_WARNING(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            ZZ_LOG_INFO(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            ZZ_LOG_TRACE(callbackData->pMessage);
            break;
    }
    return VK_FALSE;
}
#elif defined(ZZ_RELEASE)
#endif

#endif