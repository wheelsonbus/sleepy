#include "render.h"

#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/application/application.h"
#include "backend/backend.h"

b8 render_initialize(const char* application_name, struct platform_application* platform_application)
{
    if (!backend_initialize(application_name, platform_application))
    {
        ZZ_LOG_FATAL("Failed to initialize backend.");
        return FALSE;
    }

    ZZ_LOG_INFO("Render module initialized.");
    return TRUE;
}

void render_deinitialize()
{
    backend_deinitialize();
    ZZ_LOG_INFO("Render module deinitialized.");
}

void render_resize(u16 width, u16 height)
{
    backend_resize(width, height);
}

b8 render_draw_frame(struct render_packet* packet)
{
    if (backend_begin_frame(packet->delta_time))
    {
        if (!backend_end_frame(packet->delta_time))
        {
            ZZ_LOG_ERROR("backend_end_frame failed.");
            return FALSE;
        }
    }

    return TRUE;
}

void render_get_size(u32* width, u32* height)
{
    application_get_size(width, height);
}