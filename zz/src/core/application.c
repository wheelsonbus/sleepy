#include "zz/application.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "platform/application.h"

b8 application_initialize(struct application_config* config)
{
    struct platform_application_config platform_application_config;
    platform_application_config.name = config->name;
    platform_application_config.x = config->x;
    platform_application_config.y = config->y;
    platform_application_config.width = config->width;
    platform_application_config.height = config->height;
    if (!platform_application_initialize(&platform_application_config))
    {
        return ZZ_FALSE;
    }

    ZZ_LOG_INFO("Application module created.");
    return ZZ_TRUE;
}

void application_deinitialize()
{
    platform_application_deinitialize();
    ZZ_LOG_INFO("Application module destroyed.");
}

void application_get_size(u16* width, u16* height)
{
    platform_application_get_size(width, height);
}

u64 application_get_time()
{
    return platform_application_get_time();
}

void application_sleep(u64 milliseconds)
{
    platform_application_sleep(milliseconds);
}