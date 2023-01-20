#include "application.h"

#include "game.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "platform/platform.h"

struct application
{
    struct game* game;
    struct platform platform;
    b8 running;
    b8 suspended;
    i16 width;
    i16 height;
    f64 last_time;
};

static b8 application_initialized = FALSE;
static struct application application;

b8 application_initialize(struct game* game)
{
    if (application_initialized)
    {
        ZZ_LOG_ERROR("application_initialize called more than once.");
        return FALSE;
    }

    application.game = game;
    application.running = TRUE;
    application.suspended = FALSE;

    struct application_config* config = &(application.game->application_config);
    if (!platform_initialize(&application.platform, config->name, config->x, config->y, config->width, config->height))
    {
        return FALSE;
    }

    if (!application.game->initialize(application.game))
    {
        ZZ_LOG_FATAL("Game failed to initialize.");
        return FALSE;
    }
    application.game->resize(application.game, application.width, application.height);

    application_initialized = TRUE;
    return TRUE;
}

b8 application_run()
{
    ZZ_LOG_INFO(memory_get_usage_string());

    while (application.running)
    {
        if (!platform_pump_messages(&application.platform))
        {
            application.running = FALSE;
        }

        if (!application.suspended)
        {
            if (!application.game->update(application.game, 0.0f))
            {
                ZZ_LOG_FATAL("Game update failed.");
                application.running = FALSE;
                break;
            }
            
            if (!application.game->render(application.game, 0.0f))
            {
                ZZ_LOG_FATAL("Game render failed.");
                application.running = FALSE;
                break;
            }
        }
    }

    application.running = FALSE;

    platform_deinitialize(&application.platform);

    return TRUE;
}