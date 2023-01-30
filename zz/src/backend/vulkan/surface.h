#if defined(ZZ_BACKEND_VULKAN)

#pragma once

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/application.h"

b8 backend_vulkan_create_surface(VkSurfaceKHR* surface, VkInstance instance, struct application* application);
void backend_vulkan_destroy_surface(VkSurfaceKHR* surface, VkInstance instance);

#endif