#ifndef ZZ_CLIENT_H
#define ZZ_CLIENT_H

#include "zz.h"

struct client
{
    u16 width, height;
    b8 running, suspended;

    u64 accumulated_tick_time;
    u64 accumulated_frame_time;
    u64 last_frame_time;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();

    b8 (*on_tick)(u64 delta_time);
    b8 (*on_frame)(u64 delta_time);
};

struct client_config
{
    char* name;
    u16 x, y;
    u16 width, height;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();
    
    b8 (*on_tick)(u64 delta_time);
    b8 (*on_frame)(u64 delta_time);
};

ZZ_API b8 client_initialize(struct client_config* config);
ZZ_API void client_deinitialize();

ZZ_API b8 client_loop();

#endif