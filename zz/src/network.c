#include "zz/network.h"

#include "zz/log.h"
#include "zz/internal/network.h"

b8 zz_network_initialize(struct zz_network_config* config)
{
    struct zz_internal_network_config internal_network_config;
    if (!zz_internal_network_initialize(&internal_network_config))
    {
        return ZZ_TRUE;
    }

    ZZ_LOG_INFO("Network module initialized.");
    return ZZ_TRUE;
}

void zz_network_deinitialize()
{
    ZZ_LOG_INFO("Network module deinitialized.");
}