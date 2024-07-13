#ifdef ZZ_VULKAN
#ifndef ZZ_INTERNAL_VLUKAN_VERTEX_H
#define ZZ_INTERNAL_VLUKAN_VERTEX_H

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/math.h"

struct zz_internal_vulkan_vertex
{
    vec3 position;
    vec2 texture_coordinates;
};
typedef struct {struct zz_internal_vulkan_vertex* data; u16 length, capacity;} zz_memory_array_internal_vulkan_vertex_t;

struct zz_internal_vulkan_uniform_buffer_object
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

#endif
#endif