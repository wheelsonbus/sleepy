#ifdef ZZ_VULKAN

#include "device.h"

#include "zz/log.h"

b8 zz_internal_vulkan_device_create(struct zz_internal_vulkan_device* device, struct zz_internal_vulkan_device_config* config)
{
    device->instance = config->instance;
    device->surface = config->surface;

    zz_internal_vulkan_device_select_physical_device(device);

    float queuePriority = 1.0f;
    uint32_t uniqueQueueFamilyIndexCount = 0;
    uint32_t uniqueQueueFamilyIndices[2];
    if (ZZ_TRUE)
    {
        uniqueQueueFamilyIndices[uniqueQueueFamilyIndexCount++] = device->queue_family_indices.graphics;
    }
    if (device->queue_family_indices.present != device->queue_family_indices.graphics)
    {
        uniqueQueueFamilyIndices[uniqueQueueFamilyIndexCount++] = device->queue_family_indices.present;
    }
    VkDeviceQueueCreateInfo deviceQueueCreateInfos[uniqueQueueFamilyIndexCount];
    for (uint32_t i = 0; i < uniqueQueueFamilyIndexCount; i += 1)
    {
        deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfos[i].pNext = ZZ_NULL;
        deviceQueueCreateInfos[i].flags = 0;
        deviceQueueCreateInfos[i].queueFamilyIndex = uniqueQueueFamilyIndices[i];
        deviceQueueCreateInfos[i].queueCount = 1;
        deviceQueueCreateInfos[i].pQueuePriorities = &queuePriority;
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = ZZ_NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = uniqueQueueFamilyIndexCount;
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = ZZ_NULL;
    deviceCreateInfo.enabledExtensionCount = 1;
    const char* enabledExtensionNames[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    deviceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if (vkCreateDevice(device->physicalDevice, &deviceCreateInfo, ZZ_NULL, &device->device) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    vkGetDeviceQueue(device->device, device->queue_family_indices.graphics, 0, &device->graphicsQueue);
    vkGetDeviceQueue(device->device, device->queue_family_indices.present, 0, &device->presentQueue);

    return ZZ_TRUE;
}

void zz_internal_vulkan_device_destroy(struct zz_internal_vulkan_device* device)
{
    vkDestroyDevice(device->device, ZZ_NULL);
    device->device = VK_NULL_HANDLE;
}

b8 zz_internal_vulkan_device_select_physical_device(struct zz_internal_vulkan_device* device)
{
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(device->instance->instance, &physicalDeviceCount, ZZ_NULL);
    if (physicalDeviceCount == 0)
    {
        return ZZ_FALSE;
    }
    VkPhysicalDevice physicalDevices[physicalDeviceCount];
    vkEnumeratePhysicalDevices(device->instance->instance, &physicalDeviceCount, physicalDevices);

    u16 maxScore = 0;
    for (uint32_t i = 0; i < physicalDeviceCount; i += 1)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &memoryProperties);

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, ZZ_NULL);
        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies);

        uint32_t graphicsIndex, presentIndex;
        for (uint32_t j = 0; j < queueFamilyCount; j += 1)
        {
            if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsIndex = j;
            }

            VkBool32 presentSupported = ZZ_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, device->surface->surface, &presentSupported);
            if (presentSupported)
            {
                presentIndex = j;
            }
        }

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[i], device->surface->surface, &device->swapchain_support_details.surfaceCapabilities);

        uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], device->surface->surface, &surfaceFormatCount, ZZ_NULL);
        if (surfaceFormatCount == 0)
        {
            break;
        }
        VkSurfaceFormatKHR surfaceFormats[surfaceFormatCount];
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], device->surface->surface, &surfaceFormatCount, surfaceFormats);
        b8 surface_format_available = ZZ_FALSE;
        for (uint32_t i = 0; i < surfaceFormatCount; i += 1)
        {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                device->swapchain_support_details.surfaceFormat = surfaceFormats[i];
                surface_format_available = ZZ_TRUE;
                break;
            }
        }
        if (!surface_format_available)
        {
            device->swapchain_support_details.surfaceFormat = surfaceFormats[0];
        }
        
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[i], device->surface->surface, &presentModeCount, ZZ_NULL);
        if (presentModeCount == 0)
        {
            break;
        }
        VkPresentModeKHR presentModes[presentModeCount];
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[i], device->surface->surface, &presentModeCount, presentModes);
        b8 preferred_present_mode_available = ZZ_FALSE;
        for (uint32_t i = 0; i < presentModeCount; i += 1)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                device->swapchain_support_details.presentMode = presentModes[i];
                preferred_present_mode_available = ZZ_TRUE;
                break;
            }
        }
        if (!preferred_present_mode_available)
        {
            device->swapchain_support_details.presentMode = presentModes[0];
        }

        // TODO Score physical devices
        device->physicalDevice = physicalDevices[i];
        device->queue_family_indices.graphics = graphicsIndex;
        device->queue_family_indices.present = presentIndex;
        maxScore = 1;
        break;
    }

    if (maxScore == 0)
    {
        return ZZ_FALSE;
    }
    return ZZ_TRUE;
}

b8 zz_internal_vulkan_device_select_memory_type_index(struct zz_internal_vulkan_device* device, uint32_t* memoryTypeIndex, uint32_t memoryTypeIndexFilter, VkMemoryPropertyFlags memoryPropertyFlags)
{
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device->physicalDevice, &physicalDeviceMemoryProperties);

    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i += 1)
    {
        if (memoryTypeIndexFilter & (1 << i) && (memoryPropertyFlags & physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags) == memoryPropertyFlags)
        {
            *memoryTypeIndex = i;
            return ZZ_TRUE;
        }
    }

    return ZZ_FALSE;
}

#endif