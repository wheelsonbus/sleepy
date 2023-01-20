#include "application.h"

#include "game.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/event/event.h"
#include "core/input/input.h"
#include "core/clock/clock.h"
#include "platform/application/application.h"
#include "platform/time/time.h"

struct application
{
    struct game* game;
    struct platform_application platform_application;
    b8 running;
    b8 suspended;
    i16 width;
    i16 height;
    struct clock clock;
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
    if (!platform_application_initialize(&application.platform_application, config->name, config->x, config->y, config->width, config->height))
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
    clock_start(&application.clock);
    clock_update(&application.clock);
    application.last_time = application.clock.elapsed_time;
    //f64 running_time = 0;
    //u8 frame_count = 0;
    f64 target_time_per_frame = 1.0f / 60;

    while (application.running)
    {
        if (!platform_application_pump_messages(&application.platform_application))
        {
            application.running = FALSE;
        }

        if (!application.suspended)
        {
            clock_update(&application.clock);
            f64 delta_time = application.clock.elapsed_time - application.last_time;
            f64 frame_start_time = platform_time_get();

            input_update(delta_time);

            if (!application.game->update(application.game, delta_time))
            {
                ZZ_LOG_FATAL("Game update failed.");
                application.running = FALSE;
                break;
            }
            
            if (!application.game->render(application.game, delta_time))
            {
                ZZ_LOG_FATAL("Game render failed.");
                application.running = FALSE;
                break;
            }

            f64 current_time = platform_time_get();
            f64 frame_elapsed_time = current_time - frame_start_time;
            //running_time += frame_elapsed_time;
            f64 remaining_time = target_time_per_frame - frame_elapsed_time;

            if (remaining_time > 0)
            {
                u64 remaining_time_milliseconds = remaining_time * 1000;
                b8 limit_frames = FALSE;
                if (remaining_time_milliseconds > 0 && limit_frames)
                {
                    platform_time_sleep(remaining_time_milliseconds);
                }

                //frame_count += 1;
            }

            application.last_time = current_time;
        }
    }
    application.running = FALSE;

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, 0, application_on_quit);
    event_unregister_receiver(ZZ_EVENT_CODE_KEY_PRESS, 0, application_on_key_press);

    platform_application_deinitialize(&application.platform_application);

    return TRUE;
}