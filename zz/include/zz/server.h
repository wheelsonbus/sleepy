#ifndef ZZ_SERVER_H
#define ZZ_SERVER_H

#include "zz.h"
#include "network.h"

struct zz_server
{
    b8 running;

    u64 accumulated_tick_time;
    u64 accumulated_frame_time;
    u64 last_frame_time;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();

    b8 (*on_tick)(u64 delta_time);

    b8 (*on_packet)(struct zz_network_packet* packet);
};

struct zz_server_config
{
    b8 (*on_initialize)();
    b8 (*on_deinitialize)();
    
    b8 (*on_tick)(u64 delta_time);
    
    b8 (*on_packet)(struct zz_network_packet* packet);
};

ZZ_API b8 zz_server_initialize(struct zz_server_config* config);
ZZ_API void zz_server_deinitialize();

ZZ_API b8 zz_server_loop();

#endif