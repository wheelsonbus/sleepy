#include "zz/application.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "platform/application.h"

b8 application_create(struct application* application, struct application_config* config)
{
    application->memory = config->memory;
    application->event = config->event;
    application->input = config->input;

    struct platform_application_config platform_application_config;
    platform_application_config.event = config->event;
    platform_application_config.input = config->input;
    platform_application_config.name = config->name;
    platform_application_config.x = config->x;
    platform_application_config.y = config->y;
    platform_application_config.width = config->width;
    platform_application_config.height = config->height;
    if (!platform_application_create(&application->platform_application, &platform_application_config))
    {
        return FALSE;
    }

    ZZ_LOG_INFO("Application module created.");
    return TRUE;
}

void application_destroy(struct application* application)
{
    platform_application_destroy(&application->platform_application);
    ZZ_LOG_INFO("Application module destroyed.");
}

void application_get_size(struct application* application, u16* width, u16* height)
{
    platform_application_get_size(&application->platform_application, width, height);
}

u64 application_get_time(struct application* application)
{
    return platform_application_get_time(&application->platform_application);
}

void application_sleep(struct application* application, u64 milliseconds)
{
    platform_application_sleep(&application->platform_application, milliseconds);
}