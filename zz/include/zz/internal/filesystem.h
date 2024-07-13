#ifndef ZZ_INTERNAL_FILESYSTEM_H
#define ZZ_INTERNAL_FILESYSTEM_H

#include "zz/zz.h"
#include "zz/filesystem.h"

struct zz_internal_filesystem_config
{

};

b8 zz_internal_filesystem_initialize(struct zz_internal_filesystem_config* config);
void zz_internal_filesystem_deinitialize();

u64 zz_internal_filesystem_get_size(char* path);
void zz_internal_filesystem_load(char* path, void* destination, u64 size);

#endif