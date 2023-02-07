#ifndef ZZ_INTERNAL_NETWORK_H
#define ZZ_INTERNAL_NETWORK_H

#include "zz/zz.h"

struct zz_internal_network
{

};

struct zz_internal_network_config
{

};

b8 zz_internal_network_initialize(struct zz_internal_network_config* config);
void zz_internal_network_deinitialize();

#endif