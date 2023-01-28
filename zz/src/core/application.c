#include "zz/application.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "platform/application.h"

b8 application_create(struct application* application, struct application_config* config)
{
    application->memory = config->memory;
    application->event = config->event;
    application->input = config->input;

    if (!platform_application_initialize(&application->platform_application, config->name, config->x, config->y, config->width, config->height, application->event, application->input))
    {
        return FALSE;
    }

    ZZ_LOG_INFO("Application module created.");
    return TRUE;
}

void application_destroy(struct application* application)
{
    ZZ_LOG_INFO("Application module destroyed.");
}

u64 application_get_time(struct application* application)
{
    return platform_application_get_time(&application->platform_application);
}

void application_sleep(struct application* application, u64 milliseconds)
{
    platform_application_sleep(&application->platform_application, milliseconds);
}