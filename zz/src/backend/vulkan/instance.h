#pragma once

#include "zz/zz.h"

#include "vulkan/vulkan.h"

b8 backend_vulkan_create_instance(VkInstance* instance);
void backend_vulkan_destroy_instance(VkInstance* instance);