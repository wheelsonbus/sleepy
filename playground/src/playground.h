#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <zz/zz.h>

b8 playground_on_initialize();
b8 playground_on_deinitialize();
b8 playground_on_tick(u64 delta_time);
b8 playground_on_frame(u64 delta_time);

#endif