#include "zz/render.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/application.h"

b8 render_on_resize(void* sender, void* receiver, union event_data data)
{
    struct render* render = (struct render*)receiver;
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != 0 && height != 0)
    {
        backend_render_resize(&render->backend_render);
    }

    return FALSE;
}

b8 render_create(struct render* render, struct render_config* config)
{
    render->program = config->program;
    render->memory = config->memory;
    render->event = config->event;
    render->application = config->application;

    struct backend_render_config backend_render_config;
    backend_render_config.memory = config->memory;
    backend_render_config.application = config->application;
    if (!backend_render_create(&render->backend_render, &backend_render_config))
    {
        ZZ_LOG_FATAL("Failed to create backend render module.");
        return FALSE;
    }

    event_register_receiver(render->event, ZZ_EVENT_CODE_RESIZE, render, render_on_resize);

    ZZ_LOG_INFO("Render module created.");
    return TRUE;
}

void render_destroy(struct render* render)
{
    event_unregister_receiver(render->event, ZZ_EVENT_CODE_RESIZE, render, render_on_resize);

    backend_render_destroy(&render->backend_render);

    ZZ_LOG_INFO("Render module destroyed.");
}

b8 render_draw_frame(struct render* render, struct render_packet* packet)
{
    if (!backend_render_draw_frame(&render->backend_render))
    {
        return FALSE;
    }

    return TRUE;
}