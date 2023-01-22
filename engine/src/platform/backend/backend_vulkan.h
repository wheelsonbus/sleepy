#pragma once

#include "zz.h"

#include "utility/dynamic_array/dynamic_array.h"
#include "platform/core/application/application.h"
#include "backend/backend_vulkan.h"

void platform_backend_vulkan_dynamic_array_push_required_extension_names(const char*** dynamic_array);
b8 platform_backend_vulkan_context_create_surface(struct platform_application* application, struct backend_vulkan_context* context);