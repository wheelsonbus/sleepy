#include <zz/log.h>
#include <zz/client.h>

#include "client.h"

int main()
{
    struct zz_client_config client_config;
    client_config.name = "Playground Client";
    client_config.x = 560;
    client_config.y = 80;
    client_config.width = 1280;
    client_config.height = 720;
    client_config.on_initialize = client_on_initialize;
    client_config.on_deinitialize = client_on_deinitialize;
    client_config.on_tick = client_on_tick;
    client_config.on_frame = client_on_frame;
    
    if (!zz_client_initialize(&client_config))
    {
        ZZ_LOG_FATAL("Failed to create client.");
        return -1;
    }

    if (!zz_client_loop())
    {
        ZZ_LOG_ERROR("client_loop returned ZZ_FALSE.");
    }

    zz_client_deinitialize();

    return 0;
}