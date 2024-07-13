#ifndef ZZ_INTERNAL_PALETTE_H
#define ZZ_INTERNAL_PALETTE_H

#include "zz/zz.h"

struct zz_internal_palette
{
    void* data;
};

struct zz_internal_palette_config
{
    char* path;
};

b8 zz_internal_palette_create(struct zz_internal_palette* palette, struct zz_internal_palette_config* config);
void zz_internal_palette_destroy(struct zz_internal_palette* palette);

#endif
