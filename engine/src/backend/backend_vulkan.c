#include "backend.h"
#include "backend_vulkan.h"

#if defined(ZZ_BACKEND_VULKAN)

#include "utility/dynamic_array/dynamic_array.h"
#include "core/string/string.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "platform/backend/vulkan/vulkan.h"

static struct backend_vulkan_context backend_vulkan_context;

b8 backend_initialize(const char* application_name, struct platform_application* platform_application)
{
    // TODO: Custom allocator
    backend_vulkan_context.allocator = 0;

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

    if (!platform_backend_vulkan_create_surface(platform_application, &backend_vulkan_context))
    {
        ZZ_LOG_FATAL("Failed to create Vulkan surface.");
        return FALSE;
    }

    if (!backend_vulkan_create_device(&backend_vulkan_context))
    {
        ZZ_LOG_FATAL("Failed to create vulkan_device.");
        return FALSE;
    }

    return TRUE;
}

void backend_deinitialize()
{
#if defined(ZZ_DEBUG)
    if (backend_vulkan_context.debugUtilsMessenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(backend_vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyDebugUtilsMessengerEXT(backend_vulkan_context.instance, backend_vulkan_context.debugUtilsMessenger, backend_vulkan_context.allocator);
    }

    vkDestroyInstance(backend_vulkan_context.instance, backend_vulkan_context.allocator);
#elif defined(ZZ_RELEASE)
#endif
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

    ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &swapchain_support_info->surface_present_mode_count, 0));
    if (swapchain_support_info->surface_format_count != 0)
    {
        if (!swapchain_support_info->surfacePresentModes)
        {
            swapchain_support_info->surfacePresentModes = memory_allocate(sizeof(VkPresentModeKHR) * swapchain_support_info->surface_present_mode_count, ZZ_MEMORY_TAG_RENDER);
        }
        ZZ_BACKEND_VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &swapchain_support_info->surface_present_mode_count, swapchain_support_info->surfacePresentModes));
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

    ZZ_LOG_INFO("GRAPHICS | PRESENT | COMPUTE | TRANSFER | NAME");
    ZZ_LOG_INFO("     %3u |     %3u |     %3u |      %3u | %s", queue_family_info->graphics_index, queue_family_info->present_index, queue_family_info->compute_index, queue_family_info->transfer_index, properties->deviceName);

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
    if (swapchain_support_info->surface_format_count == 0 || swapchain_support_info->surface_present_mode_count == 0)
    {
        if (swapchain_support_info->surfaceFormats)
        {
            memory_deallocate(swapchain_support_info->surfaceFormats, sizeof(VkSurfaceFormatKHR) * swapchain_support_info->surface_format_count, ZZ_MEMORY_TAG_RENDER);
        }
        if (swapchain_support_info->surfacePresentModes)
        {
            memory_deallocate(swapchain_support_info->surfacePresentModes, sizeof(VkPresentModeKHR) * swapchain_support_info->surface_present_mode_count, ZZ_MEMORY_TAG_RENDER);
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

b8 backend_vulkan_select_physical_device(struct backend_vulkan_context* context)
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

        struct backend_vulkan_physical_device_requirements requirements = {};
        requirements.graphics = TRUE;
        requirements.present = TRUE;
        //requirements.compute = TRUE;
        requirements.transfer = TRUE;
        requirements.sampler_anisotropy = TRUE;
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

b8 backend_vulkan_create_device(struct backend_vulkan_context* context)
{
    if (!backend_vulkan_select_physical_device(context))
    {
        return FALSE;
    }

    return TRUE;
}

void backend_vulkan_destroy_device(struct backend_vulkan_context* context)
{

}

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    switch (messageSeverity)
    {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ZZ_LOG_FATAL(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            ZZ_LOG_ERROR(callbackData->pMessage);
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