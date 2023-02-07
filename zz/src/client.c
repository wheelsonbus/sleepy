#include "zz/client.h"

#include "zz/log.h"

#include "zz/math.h"

#define ZZ_MILLISECONDS_PER_TICK 10
#define ZZ_MILLISECONDS_PER_FRAME 0

static struct client client;

b8 client_on_quit(void* sender, void* receiver, union event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    client.running = ZZ_FALSE;
    return ZZ_TRUE;
}

b8 client_on_resize(void* sender, void* receiver, union event_data data)
{
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != client.width || height != client.height)
    {
        client.width = width;
        client.height = height;

        if (width == 0 || height == 0)
        {
            ZZ_LOG_INFO("Window minimized. Suspending.");
            client.suspended = ZZ_TRUE;
            return ZZ_TRUE;
        }
        else
        {
            if (client.suspended)
            {
                ZZ_LOG_INFO("Window restored. Resuming.");
                client.suspended = ZZ_FALSE;
            }
        }
    }

    return ZZ_FALSE;
}

b8 client_on_key_press(void* sender, void* receiver, union event_data data)
{
    u16 code = data.u16[0];
    ZZ_LOG_DEBUG("%c", code);
    if (code == ZZ_INPUT_KEY_CODE_ESCAPE)
    {
        event_send_null(ZZ_EVENT_CODE_QUIT, 0);
    }

    return ZZ_FALSE;
}

b8 client_initialize(struct client_config* config)
{
    client.on_initialize = config->on_initialize;
    client.on_deinitialize = config->on_deinitialize;
    client.on_tick = config->on_tick;
    client.on_frame = config->on_frame;
    client.width = config->width;
    client.height = config->height;
    
    client.running = ZZ_TRUE;
    client.suspended = ZZ_FALSE;

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

    event_register_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, client_on_quit);
    event_register_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, client_on_resize);
    event_register_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, client_on_key_press);

    if (!client.on_initialize())
    {
        return ZZ_FALSE;
    }

    union event_data event_data;
    event_data.u16[0] = config->width;
    event_data.u16[1] = config->height;
    event_send(ZZ_EVENT_CODE_RESIZE, 0, event_data);

    return ZZ_TRUE;
}

void client_deinitialize()
{
    if (!client.on_deinitialize())
    {
        ZZ_LOG_ERROR("Client deinitialize method returned ZZ_FALSE.");
    }

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, client_on_quit);
    event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, client_on_resize);
    event_unregister_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, client_on_key_press);

    render_deinitialize();
    application_deinitialize();
    input_deinitialize();
    event_deinitialize();
    memory_deinitialize();
}

b8 client_loop()
{
    client.last_frame_time = application_get_time();
    client.accumulated_tick_time = 0;
    client.accumulated_frame_time = 0;

    while (client.running)
    {
        if (client.suspended)
        {
            application_sleep(ZZ_MILLISECONDS_PER_TICK);
            client.last_frame_time = application_get_time();
        }
        else
        {
            u64 delta_time = application_get_time() - client.last_frame_time;
            client.last_frame_time += delta_time;
            client.accumulated_tick_time += delta_time;
            client.accumulated_frame_time += delta_time;

            while (client.accumulated_tick_time >= ZZ_MILLISECONDS_PER_TICK)
            {
                input_update();
                client.on_tick(ZZ_MILLISECONDS_PER_TICK);
                client.accumulated_tick_time -= ZZ_MILLISECONDS_PER_TICK;
            }

            if (client.accumulated_frame_time >= ZZ_MILLISECONDS_PER_FRAME)
            {
                client.on_frame(client.accumulated_frame_time);
                client.accumulated_frame_time = 0;
            }
            render_draw_frame();
        }
        internal_application_pump_messages();
    }

    return ZZ_TRUE;
}