#ifndef SERVER_H
#define SERVER_H

#include <zz/zz.h>

b8 server_on_initialize();
b8 server_on_deinitialize();
b8 server_on_tick(u64 delta_time);

#endif