#include "zz/client.h"

#include "zz/log.h"
#include "zz/math.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/input.h"
#include "zz/application.h"
#include "zz/render.h"
#include "zz/network.h"

#define ZZ_MILLISECONDS_PER_TICK 10
#define ZZ_MILLISECONDS_PER_FRAME 0

static struct zz_client client;

static b8 on_quit(void* sender, void* receiver, union zz_event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    client.running = ZZ_FALSE;
    return ZZ_TRUE;
}

static b8 on_resize(void* sender, void* receiver, union zz_event_data data)
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

static b8 on_key_press(void* sender, void* receiver, union zz_event_data data)
{
    u16 code = data.u16[0];
    ZZ_LOG_DEBUG("%c", code);
    if (code == ZZ_INPUT_KEY_CODE_ESCAPE)
    {
        zz_event_send_null(ZZ_EVENT_CODE_QUIT, 0);
    }

    return ZZ_FALSE;
}

b8 zz_client_initialize(struct zz_client_config* config)
{
    client.width = config->width;
    client.height = config->height;
    client.server_ip_endpoint = config->server_ip_endpoint;
    client.on_initialize = config->on_initialize;
    client.on_deinitialize = config->on_deinitialize;
    client.on_tick = config->on_tick;
    client.on_frame = config->on_frame;
    client.on_packet = config->on_packet;
    
    client.running = ZZ_TRUE;
    client.suspended = ZZ_FALSE;

    struct zz_log_config log_config;
    if (!zz_log_initialize(&log_config))
    {
        ZZ_LOG_FATAL("Failed to initialize log module.");
        return ZZ_FALSE;
    }

    struct zz_memory_config memory_config;
    if (!zz_memory_initialize(&memory_config))
    {
        ZZ_LOG_FATAL("Failed to initialize memory module.");
        return ZZ_FALSE;
    }

    struct zz_event_config event_config;
    if (!zz_event_initialize(&event_config))
    {
        ZZ_LOG_FATAL("Failed to initialize event module.");
        return ZZ_FALSE;
    }

    struct zz_input_config input_config;
    if (!zz_input_initialize(&input_config))
    {
        ZZ_LOG_FATAL("Failed to initialize input module.");
        return ZZ_FALSE;
    }

    struct zz_application_config application_config;
    application_config.name = config->name;
    application_config.x = config->x;
    application_config.y = config->y;
    application_config.width = config->width;
    application_config.height = config->height;
    if (!zz_application_initialize(&application_config))
    {
        ZZ_LOG_FATAL("Failed to initialize application module.");
        return ZZ_FALSE;
    }

    struct zz_render_config render_config;
    if (!zz_render_initialize(&render_config))
    {
        ZZ_LOG_FATAL("Failed to initialize render module.");
        return ZZ_FALSE;
    }

    struct zz_network_config network_config;
    network_config.ip_endpoint = zz_network_ip_endpoint_fill(127, 0, 0, 1, 9091);
    if (!zz_network_initialize(&network_config))
    {
        ZZ_LOG_FATAL("Failed to initialize network module.");
        return ZZ_FALSE;
    }

    zz_event_register_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, on_quit);
    zz_event_register_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, on_resize);
    zz_event_register_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, on_key_press);

    if (!client.on_initialize())
    {
        return ZZ_FALSE;
    }

    union zz_event_data event_data;
    event_data.u16[0] = config->width;
    event_data.u16[1] = config->height;
    zz_event_send(ZZ_EVENT_CODE_RESIZE, 0, event_data);

    return ZZ_TRUE;
}

void zz_client_deinitialize()
{
    if (!client.on_deinitialize())
    {
        ZZ_LOG_ERROR("Client deinitialize method returned ZZ_FALSE.");
    }

    zz_event_unregister_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, on_quit);
    zz_event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, ZZ_NULL, on_resize);
    zz_event_unregister_receiver(ZZ_EVENT_CODE_KEY_PRESS, ZZ_NULL, on_key_press);

    zz_render_deinitialize();
    zz_application_deinitialize();
    zz_input_deinitialize();
    zz_event_deinitialize();
    zz_memory_deinitialize();
}

b8 zz_client_loop()
{
    client.last_frame_time = zz_application_get_time();
    client.accumulated_tick_time = 0;
    client.accumulated_frame_time = 0;

    while (client.running)
    {
        if (client.suspended)
        {
            zz_application_sleep(ZZ_MILLISECONDS_PER_TICK);
            client.last_frame_time = zz_application_get_time();
        }
        else
        {
            u64 delta_time = zz_application_get_time() - client.last_frame_time;
            client.last_frame_time += delta_time;
            client.accumulated_tick_time += delta_time;
            client.accumulated_frame_time += delta_time;

            while (client.accumulated_tick_time >= ZZ_MILLISECONDS_PER_TICK)
            {  
                struct zz_network_packet packet;
                while (zz_network_receive(&packet))
                {
                    client.on_packet(&packet);
                }
                zz_input_update();
                client.on_tick(ZZ_MILLISECONDS_PER_TICK);
                client.accumulated_tick_time -= ZZ_MILLISECONDS_PER_TICK;
            }

            if (client.accumulated_frame_time >= ZZ_MILLISECONDS_PER_FRAME)
            {
                client.on_frame(client.accumulated_frame_time);
                client.accumulated_frame_time = 0;
            }
            zz_render_draw_frame();
        }
        zz_internal_application_pump_messages();
    }

    return ZZ_TRUE;
}

ZZ_API struct zz_network_ip_endpoint zz_client_get_server_ip_endpoint()
{
    return client.server_ip_endpoint;
}