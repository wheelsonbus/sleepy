#include "backend.h"
#include "backend_vulkan.h"

#if defined(ZZ_BACKEND_VULKAN)

#include "utility/dynamic_array/dynamic_array.h"
#include "core/string/string.h"
#include "core/log/log.h"
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
    ZZ_LOG_DEBUG("Required Vulkan extensions:");
    for (u32 i = 0; i < required_extension_count; i += 1)
    {
        ZZ_LOG_DEBUG("  %s", required_extension_names[i]);
    }

    const char** required_layer_names = dynamic_array_create(const char*);
    dynamic_array_push(required_layer_names, &"VK_LAYER_KHRONOS_validation");
    u32 required_layer_count = dynamic_array_get_length(required_layer_names);
    ZZ_LOG_DEBUG("Required Vulkan layers:");
    for (u32 i = 0; i < required_layer_count; i += 1)
    {
        ZZ_LOG_DEBUG("  %s", required_layer_names[i]);
    }

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