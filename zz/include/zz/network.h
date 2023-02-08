#ifndef ZZ_NETWORK_H
#define ZZ_NETWORK_H

#include "zz.h"

struct zz_network_ip_endpoint
{
    u32 address;
    u32 port;
};

ZZ_API struct zz_network_ip_endpoint zz_network_ip_endpoint_fill(u8 a, u8 b, u8 c, u8 d, u32 port);
ZZ_API b8 zz_network_ip_endpoint_equals(const struct zz_network_ip_endpoint* a, const struct zz_network_ip_endpoint* b);

struct zz_network_packet
{
    u8 buffer[1024];
    u32 size;
    u64 time;
    struct zz_network_ip_endpoint ip_endpoint;
};
typedef struct {struct zz_network_packet* data; u16 length, capacity;} zz_memory_array_network_packet_t;

struct zz_network
{
    struct zz_network_ip_endpoint ip_endpoint;
};

struct zz_network_config
{
    struct zz_network_ip_endpoint ip_endpoint;
};

ZZ_API b8 zz_network_initialize(struct zz_network_config* config);
ZZ_API void zz_network_deinitialize();

ZZ_API b8 zz_network_send(struct zz_network_packet* packet);
ZZ_API b8 zz_network_receive(struct zz_network_packet* packet);

#endif