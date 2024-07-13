#ifndef ZZ_PALETTE_H
#define ZZ_PALETTE_H

#include "zz.h"

struct zz_color
{
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};

struct zz_palette_file_header
{
    u16 width, height;
};

struct zz_palette_file_body_element
{
    struct zz_color color;
};

struct zz_palette_file
{
    u64 size;
    void* data;
    struct zz_palette_file_header* header;
    struct zz_palette_file_body_element* body;
};

struct zz_palette_file_config
{
    char* path;
};

b8 zz_palette_file_create(struct zz_palette_file* palette_file, struct zz_palette_file_config* config);
void zz_palette_file_destroy(struct zz_palette_file* palette_file);

struct zz_palette
{
    struct zz_palette_file file;
};

struct zz_palette_config
{
    char* path;
};

ZZ_API b8 zz_palette_create(struct zz_palette* palette, struct zz_palette_config* config);
ZZ_API void zz_palette_destroy(struct zz_palette* palette);

#endif