#include "zz/render.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/application.h"

//static struct render render;

static b8 on_resize(void* sender, void* receiver, union zz_event_data data)
{
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != 0 && height != 0)
    {
        zz_internal_render_resize();
    }

    return ZZ_FALSE;
}

b8 zz_render_initialize(struct zz_render_config* config)
{
    struct zz_internal_render_config internal_render_config;
    if (!zz_internal_render_initialize(&internal_render_config))
    {
        ZZ_LOG_FATAL("Failed to create internal render module.");
        return ZZ_FALSE;
    }

    zz_event_register_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, on_resize);

    ZZ_LOG_INFO("Render module deinitialized.");
    return ZZ_TRUE;
}

void zz_render_deinitialize()
{
    zz_event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, on_resize);

    zz_internal_render_deinitialize();

    ZZ_LOG_INFO("Render module initialized.");
}

b8 zz_render_draw_frame()
{
    if (!zz_internal_render_draw_frame())
    {
        return ZZ_FALSE;
    }
    return ZZ_TRUE;
}

void zz_render_set_model_matrix(mat4 matrix)
{
    zz_internal_render_set_model_matrix(matrix);
}

void zz_render_set_view_matrix(mat4 matrix)
{
    zz_internal_render_set_view_matrix(matrix);
}

void zz_render_set_projection_matrix(mat4 matrix)
{
    zz_internal_render_set_projection_matrix(matrix);
}

void zz_render_draw_sprite(struct zz_sprite* sprite, vec3 position)
{
    zz_internal_render_draw_sprite(sprite, position);
}