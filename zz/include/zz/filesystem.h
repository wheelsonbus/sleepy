#ifndef ZZ_FILESYSTEM_H
#define ZZ_FILESYSTEM_H

#include "zz.h"

struct zz_filesystem
{

};

struct zz_filesystem_config
{
    
};

ZZ_API b8 zz_filesystem_initialize(struct zz_filesystem_config* config);
ZZ_API void zz_filesystem_deinitialize();

ZZ_API u64 zz_filesystem_get_size(char* path);
ZZ_API void zz_filesystem_load(char* path, void* destination, u64 size);

ZZ_API b8 zz_filesystem_open(char* path, void** file);
ZZ_API void zz_filesystem_close(void* file);

ZZ_API b8 zz_filesystem_read_line(void* file, char* line, u64 max);

#endif