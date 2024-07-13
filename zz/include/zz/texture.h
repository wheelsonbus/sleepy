#ifndef ZZ_TEXTURE_H
#define ZZ_TEXTURE_H

#include "zz.h"

struct zz_texture_file_header
{
    u16 width, height;
};

struct zz_texture_file_body_element
{
    u32 index;
};

struct zz_texture_file
{
    u64 size;
    void* data;
    struct zz_texture_file_header* header;
    struct zz_texture_file_body_element* body;
};

struct zz_texture_file_config
{
    char* path;
};

b8 zz_texture_file_create(struct zz_texture_file* texture_file, struct zz_texture_file_config* config);
void zz_texture_file_destroy(struct zz_texture_file* texture_file);

struct zz_texture
{
    struct zz_texture_file file;
};

struct zz_texture_config
{
    char* path;
};

ZZ_API b8 zz_texture_create(struct zz_texture* texture, struct zz_texture_config* config);
ZZ_API void zz_texture_destroy(struct zz_texture* texture);

#endif