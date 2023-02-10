#ifndef CLIENT_H
#define CLIENT_H

#include <zz/zz.h>
#include <zz/network.h>

#include <playground/network.h>

static const u64 client_milliseconds_per_tick = NETWORK_MILLISECONDS_PER_TICK;
static const u64 client_milliseconds_per_frame = 0;

struct client
{
    u32 tick;
    u32 latency_tick;
    struct network_server_state server_state;
    struct network_state network_states[NETWORK_MAX_TICKS];
};

b8 client_on_initialize();
b8 client_on_deinitialize();
b8 client_on_tick();
b8 client_on_frame(u64 delta_time);
b8 client_on_packet(struct zz_network_packet* packet);

b8 client_go_to_tick(u32 tick);
b8 client_predict_tick(const struct network_server_state* state, struct network_state* predicted_state);

#endif