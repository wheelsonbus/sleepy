#ifndef ZZ_CLIENT_H
#define ZZ_CLIENT_H

#include "zz.h"
#include "network.h"

struct zz_client
{
    u16 width, height;
    b8 running, suspended;

    u64 accumulated_tick_time;
    u64 accumulated_frame_time;
    u64 last_frame_time;

    struct zz_network_ip_endpoint client_ip_endpoint;
    struct zz_network_ip_endpoint server_ip_endpoint;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();

    u64 milliseconds_per_tick;
    u64 milliseconds_per_frame;

    b8 (*on_tick)();
    b8 (*on_frame)(u64 delta_time);

    b8 (*on_packet)(struct zz_network_packet* packet);
};

struct zz_client_config
{
    char* name;
    u16 x, y;
    u16 width, height;
    
    struct zz_network_ip_endpoint client_ip_endpoint;
    struct zz_network_ip_endpoint server_ip_endpoint;

    u64 milliseconds_per_tick;
    u64 milliseconds_per_frame;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();
    
    b8 (*on_tick)();
    b8 (*on_frame)(u64 delta_time);

    b8 (*on_packet)(struct zz_network_packet* packet);
};

ZZ_API b8 zz_client_initialize(struct zz_client_config* config);
ZZ_API void zz_client_deinitialize();

ZZ_API b8 zz_client_loop();

ZZ_API struct zz_network_ip_endpoint zz_client_get_server_ip_endpoint();

#endif