#ifndef ZZ_INTERNAL_NETWORK_H
#define ZZ_INTERNAL_NETWORK_H

#include "zz/zz.h"
#include "zz/network.h"

struct zz_internal_network_config
{
    struct zz_network_ip_endpoint ip_endpoint;
};

b8 zz_internal_network_initialize(struct zz_internal_network_config* config);
void zz_internal_network_deinitialize();

b8 zz_internal_network_send(struct zz_network_packet* packet);
b8 zz_internal_network_receive(struct zz_network_packet* packet);

#endif