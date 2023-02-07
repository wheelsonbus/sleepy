#include "zz/application.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/internal/application.h"

b8 zz_application_initialize(struct zz_application_config* config)
{
    struct zz_internal_application_config internal_application_config;
    internal_application_config.name = config->name;
    internal_application_config.x = config->x;
    internal_application_config.y = config->y;
    internal_application_config.width = config->width;
    internal_application_config.height = config->height;
    if (!zz_internal_application_initialize(&internal_application_config))
    {
        return ZZ_FALSE;
    }

    ZZ_LOG_INFO("Application module initialized.");
    return ZZ_TRUE;
}

void zz_application_deinitialize()
{
    zz_internal_application_deinitialize();
    ZZ_LOG_INFO("Application module deinitialized.");
}

void zz_application_get_size(u16* width, u16* height)
{
    zz_internal_application_get_size(width, height);
}

u64 zz_application_get_time()
{
    return zz_internal_application_get_time();
}

void zz_application_sleep(u64 milliseconds)
{
    zz_internal_application_sleep(milliseconds);
}