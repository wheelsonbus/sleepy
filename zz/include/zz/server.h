#ifndef ZZ_SERVER_H
#define ZZ_SERVER_H

#include "zz.h"

struct server
{
    b8 running;

    u64 accumulated_tick_time;
    u64 accumulated_frame_time;
    u64 last_frame_time;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();

    b8 (*on_tick)(u64 delta_time);
};

struct server_config
{
    b8 (*on_initialize)();
    b8 (*on_deinitialize)();
    
    b8 (*on_tick)(u64 delta_time);
};

ZZ_API b8 server_initialize(struct server_config* config);
ZZ_API void server_deinitialize();

ZZ_API b8 server_loop();

#endif