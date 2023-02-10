#include <stdio.h>
#include <stdlib.h>

#include <zz/log.h>
#include <zz/client.h>

#include "client.h"

int main(int argc, char* argv[])
{
    u8 client_a = 127;
    u8 client_b = 0;
    u8 client_c = 0;
    u8 client_d = 1;
    u32 client_port = 9091;

    u8 server_a = 127;
    u8 server_b = 0;
    u8 server_c = 0;
    u8 server_d = 1;
    u32 server_port = 9090;

    int i, j, l;
    for (i = 0; i < argc; ++i)
    {
        switch(argv[i][0])
        {
            case '-':
            {
                switch(argv[i][1])
                {
                    case 'c':
                    {
                        ++i;
                        j = 0;
                        char* start = &argv[i][j];
                        char* end;

                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        client_a = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        client_b = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        client_c = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != ':')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        client_d = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '\0')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        client_port = strtol(start, &end, 10);
                        ++j;
                    }
                    break;

                    case 's':
                    {
                        ++i;
                        j = 0;
                        char* start = &argv[i][j];
                        char* end;

                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        server_a = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        server_b = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '.')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        server_c = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != ':')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        server_d = strtol(start, &end, 10);
                        ++j;

                        start = &argv[i][j];
                        while (argv[i][j] != '\0')
                        {
                            ++j;
                        }
                        end = &argv[i][j];
                        server_port = strtol(start, &end, 10);
                        ++j;
                    }
                    break;
                }
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
    client_config.client_ip_endpoint = zz_network_ip_endpoint_fill(client_a, client_b, client_c, client_d, client_port);
    client_config.server_ip_endpoint = zz_network_ip_endpoint_fill(server_a, server_b, server_c, server_d, server_port);
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