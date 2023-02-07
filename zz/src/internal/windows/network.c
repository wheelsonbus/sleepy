#ifdef ZZ_WINDOWS

#include "zz/internal/network.h"
#include "network.h"

#include <WinSock2.h>

//static struct internal_network internal_network;

b8 zz_internal_network_initialize(struct zz_internal_network_config* config)
{


    return ZZ_TRUE;
}

void zz_internal_network_deinitialize()
{

}

#endif