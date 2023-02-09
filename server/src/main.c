#include <zz/log.h>
#include <zz/server.h>

#include "server.h"

int main()
{
    struct zz_server_config server_config;
    server_config.milliseconds_per_tick = server_milliseconds_per_tick;
    server_config.on_initialize = server_on_initialize;
    server_config.on_deinitialize = server_on_deinitialize;
    server_config.on_tick = server_on_tick;
    server_config.on_packet = server_on_packet;
    
    if (!zz_server_initialize(&server_config))
    {
        ZZ_LOG_FATAL("Failed to create server.");
        return -1;
    }

    if (!zz_server_loop())
    {
        ZZ_LOG_ERROR("server_loop returned ZZ_FALSE.");
    }

    zz_server_deinitialize();

    return 0;
}