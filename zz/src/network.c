#include "zz/network.h"

#include "zz/log.h"

b8 network_initialize(struct network_config* config)
{
    ZZ_LOG_INFO("Network module initialized.");
    return ZZ_TRUE;
}

void network_deinitialize()
{
    ZZ_LOG_INFO("Network module deinitialized.");
}