#ifndef CLIENT_H
#define CLIENT_H

#include <zz/zz.h>
#include <zz/network.h>

b8 client_on_initialize();
b8 client_on_deinitialize();
b8 client_on_tick(u64 delta_time);
b8 client_on_frame(u64 delta_time);
b8 client_on_packet(struct zz_network_packet* packet);

#endif