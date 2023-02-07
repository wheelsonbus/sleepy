#include "zz/render.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/application.h"

//static struct render render;

b8 render_on_resize(void* sender, void* receiver, union event_data data)
{
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != 0 && height != 0)
    {
        internal_render_resize();
    }

    return ZZ_FALSE;
}

b8 render_initialize(struct render_config* config)
{
    struct internal_render_config internal_render_config;
    if (!internal_render_initialize(&internal_render_config))
    {
        ZZ_LOG_FATAL("Failed to create internal render module.");
        return ZZ_FALSE;
    }

    event_register_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, render_on_resize);

    ZZ_LOG_INFO("Render module created.");
    return ZZ_TRUE;
}

void render_deinitialize()
{
    event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, render_on_resize);

    internal_render_deinitialize();

    ZZ_LOG_INFO("Render module destroyed.");
}

b8 render_draw_frame()
{
    if (!internal_render_draw_frame())
    {
        return ZZ_FALSE;
    }
    return ZZ_TRUE;
}

void render_set_model_matrix(mat4 matrix)
{
    internal_render_set_model_matrix(matrix);
}

void render_set_view_matrix(mat4 matrix)
{
    internal_render_set_view_matrix(matrix);
}

void render_set_projection_matrix(mat4 matrix)
{
    internal_render_set_projection_matrix(matrix);
}

void render_draw_sprite(struct sprite* sprite, vec3 position)
{
    internal_render_draw_sprite(sprite, position);
}