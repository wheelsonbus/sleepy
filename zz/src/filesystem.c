#include "zz/filesystem.h"

#include "zz/log.h"
#include "zz/internal/filesystem.h"

//static struct zz_filesystem filesystem;

b8 zz_filesystem_initialize(struct zz_filesystem_config* config)
{
    struct zz_internal_filesystem_config internal_filesystem_config;
    if (!zz_internal_filesystem_initialize(&internal_filesystem_config))
    {
        ZZ_LOG_FATAL("Failed to create internal filesystem module.");
        return ZZ_FALSE;
    }

    ZZ_LOG_INFO("Filesystem module initialized.");
    return ZZ_TRUE;
}

void zz_filesystem_deinitialize()
{
    zz_internal_filesystem_deinitialize();

    ZZ_LOG_INFO("Filesystem module deinitialized.");
}

u64 zz_filesystem_get_size(char* path)
{
    return zz_internal_filesystem_get_size(path);
}

void zz_filesystem_load(char* path, void* destination, u64 size)
{
    zz_internal_filesystem_load(path, destination, size);
}