#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VLUKAN_VERTEX_H
#define ZZ_INTERNAL_VLUKAN_VERTEX_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/math.h"

struct internal_vulkan_vertex
{
    vec3 position;
};
typedef struct {struct internal_vulkan_vertex* data; u16 length, capacity;} memory_array_internal_vulkan_vertex_t;

struct internal_vulkan_uniform_buffer_object
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

#endif
#endif