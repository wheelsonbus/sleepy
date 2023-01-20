#include "application.h"

#include "game.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/event/event.h"
#include "core/input/input.h"
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

b8 application_on_quit(void* sender, void* receiver, union event_data data)
{
    ZZ_LOG_INFO("Quit event received. Shutting down application.\n");
    application.running = FALSE;
    return TRUE;
}

b8 application_on_key_press(void* sender, void* receiver, union event_data data)
{
    enum input_key_code code = data.u16[0];
    if (code == ZZ_INPUT_KEY_CODE_ESCAPE)
    {
        union event_data data;
        data.u8[0] = 0;
        event_send(ZZ_EVENT_CODE_QUIT, 0, data);
    }

    return FALSE;
}

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

    event_register_receiver(ZZ_EVENT_CODE_QUIT, 0, application_on_quit);
    event_register_receiver(ZZ_EVENT_CODE_KEY_PRESS, 0, application_on_key_press);

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
            input_update(0.0f);
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

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, 0, application_on_quit);
    event_unregister_receiver(ZZ_EVENT_CODE_KEY_PRESS, 0, application_on_key_press);

    platform_deinitialize(&application.platform);

    return TRUE;
}