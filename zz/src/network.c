#include "zz/network.h"

#include "zz/log.h"
#include "zz/internal/network.h"

static struct zz_network network;

b8 zz_network_initialize(struct zz_network_config* config)
{
    network.ip_endpoint = config->ip_endpoint;

    struct zz_internal_network_config internal_network_config;
    internal_network_config.ip_endpoint = network.ip_endpoint;
    if (!zz_internal_network_initialize(&internal_network_config))
    {
        ZZ_LOG_FATAL("Failed to create internal network module.");
        return ZZ_FALSE;
    }

    ZZ_LOG_INFO("Network module initialized.");
    return ZZ_TRUE;
}

void zz_network_deinitialize()
{
    zz_internal_network_deinitialize();

    ZZ_LOG_INFO("Network module deinitialized.");
}

b8 zz_network_send(struct zz_network_packet* packet)
{
    return zz_internal_network_send(packet);
}

b8 zz_network_receive(struct zz_network_packet* packet)
{
    return zz_internal_network_receive(packet);
}