#include <stdio.h>
#include <stdlib.h>

#include <zz/log.h>
#include <zz/client.h>

#include "client.h"

int main(int argc, char* argv[])
{

    u8 a = 127;
    u8 b = 0;
    u8 c = 0;
    u8 d = 1;
    u32 port = 9090;

    int i, j, l;
    int* p;
    for (i = 0; i < argc; ++i)
    {
        switch(argv[i][0])
        {
            case '-':
                switch(argv[i][1])
                {
                    case 's':
                        ++i;
                        j = 0;
                        char* start = &argv[i][j];
                        char* end;
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        a = strtol(start, &end, 10);
                        ++j;
                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        b = strtol(start, &end, 10);
                        ++j;
                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        c = strtol(start, &end, 10);
                        ++j;
                        start = &argv[i][j];
                        while (argv[i][j] != ':')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        d = strtol(start, &end, 10);
                        ++j;
                        start = &argv[i][j];
                        while (argv[i][j] != '\0')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        port = strtol(start, &end, 10);
                        ++j;

                        break;
                }
                break;
        }
    }

    struct zz_client_config client_config;
    client_config.name = "Playground Client";
    client_config.x = 32;
    client_config.y = 32;
    client_config.width = 640;
    client_config.height = 640;
    client_config.server_ip_endpoint = zz_network_ip_endpoint_fill(a, b, c, d, port);
    client_config.milliseconds_per_tick = client_milliseconds_per_tick;
    client_config.milliseconds_per_frame = client_milliseconds_per_frame;
    client_config.on_initialize = client_on_initialize;
    client_config.on_deinitialize = client_on_deinitialize;
    client_config.on_tick = client_on_tick;
    client_config.on_frame = client_on_frame;
    client_config.on_packet = client_on_packet;
    
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