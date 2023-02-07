#include <zz/log.h>
#include <zz/server.h>

#include "server.h"

int main()
{
    struct server_config server_config;
    server_config.on_initialize = server_on_initialize;
    server_config.on_deinitialize = server_on_deinitialize;
    server_config.on_tick = server_on_tick;
    
    if (!server_initialize(&server_config))
    {
        ZZ_LOG_FATAL("Failed to create server.");
        return -1;
    }

    if (!server_loop())
    {
        ZZ_LOG_ERROR("server_loop returned ZZ_FALSE.");
    }

    server_deinitialize();

    return 0;
}