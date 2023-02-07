#include "zz/network.h"

#include "zz/log.h"
#include "internal/network.h"

b8 network_initialize(struct network_config* config)
{
    struct internal_network_config internal_network_config;
    if (!internal_network_initialize(&internal_network_config))
    {
        return ZZ_TRUE;
    }

    ZZ_LOG_INFO("Network module initialized.");
    return ZZ_TRUE;
}

void network_deinitialize()
{
    ZZ_LOG_INFO("Network module deinitialized.");
}