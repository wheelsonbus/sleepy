#include "zz/server.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"

#include "zz/application.h" // FIXME
#include "zz/internal/application.h" // FIXME

#define ZZ_MILLISECONDS_PER_TICK 10

static struct server server;

b8 server_on_quit(void* sender, void* receiver, union event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down.\n");
    server.running = ZZ_FALSE;
    return ZZ_TRUE;
}

b8 server_initialize(struct server_config* config)
{
    server.on_initialize = config->on_initialize;
    server.on_deinitialize = config->on_deinitialize;
    server.on_tick = config->on_tick;
    
    server.running = ZZ_TRUE;

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

    event_register_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, server_on_quit);

    if (!server.on_initialize())
    {
        return ZZ_FALSE;
    }

    internal_application_initialize_time(); // FIXME

    return ZZ_TRUE;
}

void server_deinitialize()
{
    if (!server.on_deinitialize())
    {
        ZZ_LOG_ERROR("Server deinitialize method returned ZZ_FALSE.");
    }

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, ZZ_NULL, server_on_quit);

    event_deinitialize();
    memory_deinitialize();
}

b8 server_loop()
{
    server.last_frame_time = application_get_time();
    server.accumulated_tick_time = 0;

    while (server.running)
    {
        u64 delta_time = application_get_time() - server.last_frame_time;
        server.last_frame_time += delta_time;
        server.accumulated_tick_time += delta_time;

        while (server.accumulated_tick_time >= ZZ_MILLISECONDS_PER_TICK)
        {
            input_update();
            server.on_tick(ZZ_MILLISECONDS_PER_TICK);
            server.accumulated_tick_time -= ZZ_MILLISECONDS_PER_TICK;
        }
    }

    return ZZ_TRUE;
}