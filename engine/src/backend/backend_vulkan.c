#include "backend.h"
#include "backend_vulkan.h"

#if defined(ZZ_BACKEND_VULKAN)

b8 backend_initialize(struct platform_application* platform_application, const char* application_name)
{
    

    return TRUE;
}

void backend_deinitialize()
{

}

void backend_resize(u16 width, u16 height)
{

}

b8 backend_begin_frame(f32 delta_time);
b8 backend_end_frame(f32 delta_time);

#endif