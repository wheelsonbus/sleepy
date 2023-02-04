#if defined(ZZ_BACKEND_VULKAN)

#include <vulkan/vulkan.h>

#include "zz/zz.h"
#include "zz/math.h"

struct backend_vulkan_vertex
{
    vec3 position;
};
typedef struct {struct backend_vulkan_vertex* data; u16 length, capacity;} memory_array_backend_vulkan_vertex_t;

struct backend_vulkan_uniform_buffer_object
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

#endif