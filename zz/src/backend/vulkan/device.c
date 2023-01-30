#if defined(ZZ_BACKEND_VULKAN)

#include "device.h"

#include "zz/log.h"

b8 backend_vulkan_select_physical_device(VkPhysicalDevice* physicalDevice, struct queue_family_indices* queue_family_indices, struct swapchain_support_details* swapchain_support_details, VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);
    if (physicalDeviceCount == 0)
    {
        return FALSE;
    }
    VkPhysicalDevice physicalDevices[physicalDeviceCount];
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);

    u16 maxScore = 0;
    for (uint32_t i = 0; i < physicalDeviceCount; i += 1)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, NULL);
        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies);

        uint32_t graphicsIndex, presentIndex;
        for (uint32_t j = 0; j < queueFamilyCount; j += 1)
        {
            if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsIndex = j;
            }

            VkBool32 presentSupported = FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, surface, &presentSupported);
            if (presentSupported)
            {
                presentIndex = j;
            }
        }

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[i], surface, &swapchain_support_details->surfaceCapabilities);

        uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], surface, &surfaceFormatCount, NULL);
        if (surfaceFormatCount == 0)
        {
            break;
        }
        VkSurfaceFormatKHR surfaceFormats[surfaceFormatCount];
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[i], surface, &surfaceFormatCount, surfaceFormats);
        b8 surface_format_available = FALSE;
        for (uint32_t i = 0; i < surfaceFormatCount; i += 1)
        {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                swapchain_support_details->surfaceFormat = surfaceFormats[i];
                surface_format_available = TRUE;
                break;
            }
        }
        if (!surface_format_available)
        {
            swapchain_support_details->surfaceFormat = surfaceFormats[0];
        }
        
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[i], surface, &presentModeCount, NULL);
        if (presentModeCount == 0)
        {
            break;
        }
        VkPresentModeKHR presentModes[presentModeCount];
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[i], surface, &presentModeCount, presentModes);
        b8 preferred_present_mode_available = FALSE;
        for (uint32_t i = 0; i < presentModeCount; i += 1)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                swapchain_support_details->presentMode = presentModes[i];
                preferred_present_mode_available = TRUE;
                break;
            }
        }
        if (!preferred_present_mode_available)
        {
            swapchain_support_details->presentMode = presentModes[0];
        }

        // TODO Score physical devices
        *physicalDevice = physicalDevices[i];
        queue_family_indices->graphics = graphicsIndex;
        queue_family_indices->present = presentIndex;
        maxScore = 1;
        break;
    }

    if (maxScore == 0)
    {
        return FALSE;
    }
    return TRUE;
}

b8 backend_vulkan_create_device(VkDevice* device, VkQueue* graphicsQueue, VkQueue* presentQueue, VkPhysicalDevice physicalDevice, struct queue_family_indices* queue_family_indices)
{
    float queuePriority = 1.0f;
    uint32_t uniqueQueueFamilyIndexCount = 0;
    uint32_t uniqueQueueFamilyIndices[2];
    if (TRUE)
    {
        uniqueQueueFamilyIndices[uniqueQueueFamilyIndexCount++] = queue_family_indices->graphics;
    }
    if (queue_family_indices->present != queue_family_indices->graphics)
    {
        uniqueQueueFamilyIndices[uniqueQueueFamilyIndexCount++] = queue_family_indices->present;
    }
    VkDeviceQueueCreateInfo deviceQueueCreateInfos[uniqueQueueFamilyIndexCount];
    for (uint32_t i = 0; i < uniqueQueueFamilyIndexCount; i += 1)
    {
        deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfos[i].pNext = NULL;
        deviceQueueCreateInfos[i].flags = 0;
        deviceQueueCreateInfos[i].queueFamilyIndex = uniqueQueueFamilyIndices[i];
        deviceQueueCreateInfos[i].queueCount = 1;
        deviceQueueCreateInfos[i].pQueuePriorities = &queuePriority;
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = uniqueQueueFamilyIndexCount;
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount = 1;
    const char* enabledExtensionNames[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    deviceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, device) != VK_SUCCESS)
    {
        return FALSE;
    }

    vkGetDeviceQueue(*device, queue_family_indices->graphics, 0, graphicsQueue);
    vkGetDeviceQueue(*device, queue_family_indices->present, 0, presentQueue);

    return TRUE;
}

void backend_vulkan_destroy_device(VkDevice* device)
{
    vkDestroyDevice(*device, NULL);
    *device = VK_NULL_HANDLE;
}

#endif