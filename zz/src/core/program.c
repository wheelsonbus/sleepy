#include "zz/program.h"

#include "zz/log.h"

#include "zz/math.h"

b8 program_on_quit(void* sender, void* receiver, union event_data data)
{
    struct program* program = (struct program*)receiver;
    
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    program->running = FALSE;
    return TRUE;
}

b8 program_on_resize(void* sender, void* receiver, union event_data data)
{
    struct program* program = (struct program*)receiver;
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != program->width || height != program->height)
    {
        program->width = width;
        program->height = height;

        if (width == 0 || height == 0)
        {
            ZZ_LOG_INFO("Window minimized. Suspending.");
            program->suspended = TRUE;
            return TRUE;
        }
        else
        {
            if (program->suspended)
            {
                ZZ_LOG_INFO("Window restored. Resuming.");
                program->suspended = FALSE;
            }
        }
    }

    return FALSE;
}

b8 program_on_key_press(void* sender, void* receiver, union event_data data)
{
    struct program* program = (struct program*)receiver;
    u16 code = data.u16[0];
    ZZ_LOG_DEBUG("%c", code);
    if (code == ZZ_INPUT_KEY_CODE_ESCAPE)
    {
        event_send_null(&program->event, ZZ_EVENT_CODE_QUIT, 0);
    }

    return FALSE;
}

b8 program_create(struct program* program, struct program_config* config)
{
    program->on_initialize = config->on_initialize;
    program->on_deinitialize = config->on_deinitialize;
    program->on_tick = config->on_tick;
    program->on_frame = config->on_frame;
    program->width = config->width;
    program->height = config->height;
    
    program->running = TRUE;
    program->suspended = FALSE;

    struct memory_config memory_config;
    if (!memory_create(&program->memory, &memory_config))
    {
        ZZ_LOG_FATAL("Failed to create memory module.");
    }

    struct event_config event_config;
    event_config.memory = &program->memory;
    if (!event_create(&program->event, &event_config))
    {
        ZZ_LOG_FATAL("Failed to create event module.");
    }

    struct input_config input_config;
    input_config.memory = &program->memory;
    input_config.event = &program->event;
    if (!input_create(&program->input, &input_config))
    {
        ZZ_LOG_FATAL("Failed to create input module.");
    }

    struct application_config application_config;
    application_config.memory = &program->memory;
    application_config.event = &program->event;
    application_config.input = &program->input;
    application_config.name = config->name;
    application_config.x = config->x;
    application_config.y = config->y;
    application_config.width = config->width;
    application_config.height = config->height;
    if (!application_create(&program->application, &application_config))
    {
        ZZ_LOG_FATAL("Failed to create application module.");
    }

    struct render_config render_config;
    render_config.memory = &program->memory;
    render_config.event = &program->event;
    render_config.application = &program->application;
    if (!render_create(&program->render, &render_config))
    {
        ZZ_LOG_FATAL("Failed to create render module.");
        return FALSE;
    }

    event_register_receiver(&program->event, ZZ_EVENT_CODE_QUIT, program, program_on_quit);
    event_register_receiver(&program->event, ZZ_EVENT_CODE_RESIZE, program, program_on_resize);
    event_register_receiver(&program->event, ZZ_EVENT_CODE_KEY_PRESS, program, program_on_key_press);

    if (!program->on_initialize(program))
    {
        return FALSE;
    }

    union event_data event_data;
    event_data.u16[0] = config->width;
    event_data.u16[1] = config->height;
    event_send(&program->event, ZZ_EVENT_CODE_RESIZE, 0, event_data);

    return TRUE;
}

void program_destroy(struct program* program)
{
    if (!program->on_deinitialize(program))
    {
        ZZ_LOG_ERROR("Program deinitialize method returned FALSE.");
    }

    event_unregister_receiver(&program->event, ZZ_EVENT_CODE_QUIT, program, program_on_quit);
    event_unregister_receiver(&program->event, ZZ_EVENT_CODE_RESIZE, program, program_on_resize);
    event_unregister_receiver(&program->event, ZZ_EVENT_CODE_KEY_PRESS, program, program_on_key_press);

    render_destroy(&program->render);
    application_destroy(&program->application);
    input_destroy(&program->input);
    event_destroy(&program->event);
    memory_destroy(&program->memory);
}

b8 program_loop(struct program* program)
{
    while (program->running)
    {
        if (!program->suspended)
        {
            input_update(&program->input);
            program->on_tick(program, 0.1f);

            program->on_frame(program, 0.1f);
            render_draw_frame(&program->render);
        }
        platform_application_pump_messages(&program->application.platform_application);
    }

    return TRUE;
}

void program_bind_camera(struct program* program, struct camera* camera)
{
    render_bind_camera(&program->render, camera);
}

void program_draw_sprite(struct program* program, struct sprite* sprite, vec3 position)
{
    render_draw_sprite(&program->render, sprite, position);
}