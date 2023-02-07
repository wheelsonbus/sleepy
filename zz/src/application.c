#include "zz/application.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/internal/application.h"

b8 application_initialize(struct application_config* config)
{
    struct internal_application_config internal_application_config;
    internal_application_config.name = config->name;
    internal_application_config.x = config->x;
    internal_application_config.y = config->y;
    internal_application_config.width = config->width;
    internal_application_config.height = config->height;
    if (!internal_application_initialize(&internal_application_config))
    {
        return ZZ_FALSE;
    }

    ZZ_LOG_INFO("Application module initialized.");
    return ZZ_TRUE;
}

void application_deinitialize()
{
    internal_application_deinitialize();
    ZZ_LOG_INFO("Application module deinitialized.");
}

void application_get_size(u16* width, u16* height)
{
    internal_application_get_size(width, height);
}

u64 application_get_time()
{
    return internal_application_get_time();
}

void application_sleep(u64 milliseconds)
{
    internal_application_sleep(milliseconds);
}