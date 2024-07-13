#ifndef ZZ_PALETTE_H
#define ZZ_PALETTE_H

#include "zz.h"

#include "zz/internal/palette.h"

struct zz_palette
{
    struct zz_internal_palette internal_palette;
};

struct zz_palette_config
{
    char* path;
};

ZZ_API b8 zz_palette_create(struct zz_palette* palette, struct zz_palette_config* config);
ZZ_API void zz_palette_destroy(struct zz_palette* palette);

#endif