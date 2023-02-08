#include "zz/server.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/network.h"

#include "zz/application.h" // FIXME
#include "zz/internal/application.h" // FIXME

#define ZZ_MILLISECONDS_PER_TICK 10

static struct zz_server server;

static b8 on_quit(void* sender, void* receiver, union zz_event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    server.running = ZZ_FALSE;
    return ZZ_TRUE;
}

b8 zz_server_initialize(struct zz_server_config* config)
{
    server.on_initialize = config->on_initialize;
    server.on_deinitialize = config->on_deinitialize;
    server.on_tick = config->on_tick;
    server.on_packet = config->on_packet;
    
    server.running = ZZ_TRUE;

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
    
    struct zz_network_config network_config;
    network_config.ip_endpoint = zz_network_ip_endpoint_fill(127, 0, 0, 1, 9090);
    if (!zz_network_initialize(&network_config))
    {
        ZZ_LOG_FATAL("Failed to initialize network module.");
        return ZZ_FALSE;
    }

    zz_event_register_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, on_quit);

    if (!server.on_initialize())
    {
        return ZZ_FALSE;
    }

    zz_internal_application_initialize_time(); // FIXME

    return ZZ_TRUE;
}

void zz_server_deinitialize()
{
    if (!server.on_deinitialize())
    {
        ZZ_LOG_ERROR("Server deinitialize method returned ZZ_FALSE.");
    }

    zz_event_unregister_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, on_quit);

    zz_network_deinitialize();
    zz_event_deinitialize();
    zz_memory_deinitialize();
}

b8 zz_server_loop()
{
    server.last_frame_time = zz_application_get_time();
    server.accumulated_tick_time = 0;

    while (server.running)
    {
        u64 delta_time = zz_application_get_time() - server.last_frame_time;
        server.last_frame_time += delta_time;
        server.accumulated_tick_time += delta_time;

        while (server.accumulated_tick_time >= ZZ_MILLISECONDS_PER_TICK)
        {
            struct zz_network_packet packet;
            while (zz_network_receive(&packet))
            {
                server.on_packet(&packet);
            }
            server.on_tick(ZZ_MILLISECONDS_PER_TICK);
            server.accumulated_tick_time -= ZZ_MILLISECONDS_PER_TICK;
        }
    }

    return ZZ_TRUE;
}