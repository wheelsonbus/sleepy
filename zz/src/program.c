#include "zz/program.h"

#include "zz/log.h"

#include "zz/math.h"

#define ZZ_TICK_MILLISECONDS 10
#define ZZ_FRAME_MILLISECONDS 0

static struct program program;

b8 program_on_quit(void* sender, void* receiver, union event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    program.running = ZZ_FALSE;
    return ZZ_TRUE;
}

b8 program_on_resize(void* sender, void* receiver, union event_data data)
{
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != program.width || height != program.height)
    {
        program.width = width;
        program.height = height;

        if (width == 0 || height == 0)
        {
            ZZ_LOG_INFO("Window minimized. Suspending.");
            program.suspended = ZZ_TRUE;
            return ZZ_TRUE;
        }
        else
        {
            if (program.suspended)
            {
                ZZ_LOG_INFO("Window restored. Resuming.");
                program.suspended = ZZ_FALSE;
            }
        }
    }

    return ZZ_FALSE;
}

b8 program_on_key_press(void* sender, void* receiver, union event_data data)
{
    u16 code = data.u16[0];
    ZZ_LOG_DEBUG("%c", code);
    if (code == ZZ_INPUT_KEY_CODE_ESCAPE)
    {
        event_send_null(ZZ_EVENT_CODE_QUIT, 0);
    }

    return ZZ_FALSE;
}

b8 program_initialize(struct program_config* config)
{
    program.on_initialize = config->on_initialize;
    program.on_deinitialize = config->on_deinitialize;
    program.on_tick = config->on_tick;
    program.on_frame = config->on_frame;
    program.width = config->width;
    program.height = config->height;
    
    program.running = ZZ_TRUE;
    program.suspended = ZZ_FALSE;

    struct log_config log_config;
    if (!log_initialize(&log_config))
    {
        ZZ_LOG_FATAL("Failed to initialize log module.");
        return ZZ_FALSE;
    }

    struct memory_config memory_config;
    if (!memory_initialize(&memory_config))
    {
        ZZ_LOG_FATAL("Failed to initialize memory module.");
        return ZZ_FALSE;
    }

    struct event_config event_config;
    if (!event_initialize(&event_config))
    {
        ZZ_LOG_FATAL("Failed to initialize event module.");
        return ZZ_FALSE;
    }

    struct input_config input_config;
    if (!input_initialize(&input_config))
    {
        ZZ_LOG_FATAL("Failed to initialize input module.");
        return ZZ_FALSE;
    }

    struct application_config application_config;
    application_config.name = config->name;
    application_config.x = config->x;
    application_config.y = config->y;
    application_config.width = config->width;
    application_config.height = config->height;
    if (!application_initialize(&application_config))
    {
        ZZ_LOG_FATAL("Failed to initialize application module.");
        return ZZ_FALSE;
    }

    struct render_config render_config;
    if (!render_initialize(&render_config))
    {
        ZZ_LOG_FATAL("Failed to initialize render module.");
        return ZZ_FALSE;
    }

    event_register_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, program_on_quit);
    event_register_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, program_on_resize);
    event_register_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, program_on_key_press);

    if (!program.on_initialize())
    {
        return ZZ_FALSE;
    }

    union event_data event_data;
    event_data.u16[0] = config->width;
    event_data.u16[1] = config->height;
    event_send(ZZ_EVENT_CODE_RESIZE, 0, event_data);

    return ZZ_TRUE;
}

void program_deinitialize()
{
    if (!program.on_deinitialize())
    {
        ZZ_LOG_ERROR("Program deinitialize method returned ZZ_FALSE.");
    }

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, program_on_quit);
    event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, program_on_resize);
    event_unregister_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, program_on_key_press);

    render_deinitialize();
    application_deinitialize();
    input_deinitialize();
    event_deinitialize();
    memory_deinitialize();
}

b8 program_loop()
{
    program.last_frame_time = application_get_time();
    program.accumulated_tick_time = 0;
    program.accumulated_frame_time = 0;

    while (program.running)
    {
        if (program.suspended)
        {
            application_sleep(ZZ_TICK_MILLISECONDS);
            program.last_frame_time = application_get_time();
        }
        else
        {
            u64 delta_time = application_get_time() - program.last_frame_time;
            program.last_frame_time += delta_time;
            program.accumulated_tick_time += delta_time;
            program.accumulated_frame_time += delta_time;

            while (program.accumulated_tick_time >= ZZ_TICK_MILLISECONDS)
            {
                input_update();
                program.on_tick(ZZ_TICK_MILLISECONDS);
                program.accumulated_tick_time -= ZZ_TICK_MILLISECONDS;
            }

            if (program.accumulated_frame_time >= ZZ_FRAME_MILLISECONDS)
            {
                program.on_frame(program.accumulated_frame_time);
                program.accumulated_frame_time = 0;
            }
            render_draw_frame();
        }
        internal_application_pump_messages();
    }

    return ZZ_TRUE;
}