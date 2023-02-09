#ifndef SERVER_H
#define SERVER_H

#include <zz/zz.h>
#include <zz/network.h>

#include <playground/network.h>
#include <playground/box.h>

struct client
{
    struct zz_network_ip_endpoint ip_endpoint;
    u64 timeout;
    struct network_client_input inputs[NETWORK_MAX_TICKS_AHEAD];
    b8 inputs_available[NETWORK_MAX_TICKS_AHEAD];
    struct box box;
};
typedef struct {struct client* data; u16 length, capacity;} zz_memory_array_client_t;

struct server
{
    u32 tick;
    zz_memory_array_client_t clients;
};

static const u64 server_milliseconds_per_tick = NETWORK_MILLISECONDS_PER_TICK;

b8 server_on_initialize();
b8 server_on_deinitialize();
b8 server_on_tick();
b8 server_on_packet(struct zz_network_packet* packet);

#endif