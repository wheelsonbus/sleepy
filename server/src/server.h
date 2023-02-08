#ifndef SERVER_H
#define SERVER_H

#include <zz/zz.h>
#include <zz/network.h>
#include <playground/box.h>

struct client_input_state
{
    b8 left, right, up, down;
};

struct client
{
    struct zz_network_ip_endpoint ip_endpoint;
    u64 timeout;
    struct client_input_state input_state;
    struct client_input_state previous_input_state;
    struct box box;
};
typedef struct {struct client* data; u16 length, capacity;} zz_memory_array_client_t;

b8 server_on_initialize();
b8 server_on_deinitialize();
b8 server_on_tick(u64 delta_time);
b8 server_on_packet(struct zz_network_packet* packet);

#endif