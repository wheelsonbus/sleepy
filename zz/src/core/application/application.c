#include "zz/application.h"

#include "zz/program.h"
#include "zz/log.h"
#include "zz/memory.h"
#include "zz/event.h"
#include "zz/input.h"
#include "zz/time.h"
#include "zz/clock.h"
#include "zz/render.h"
#include "platform/application.h"

struct application
{
    struct program* program;
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

b8 application_on_resize(void* sender, void* receiver, union event_data data)
{
    u16 width = data.u16[0];
    u16 height = data.u16[1];

    if (width != application.width || height != application.height)
    {
        application.width = width;
        application.height = height;

        ZZ_LOG_DEBUG("Resize event received: (%i, %i)", width, height);

        if (width == 0 || height == 0)
        {
            ZZ_LOG_INFO("Window minimized. Suspending applicaiton.");
            application.suspended = TRUE;
            return TRUE;
        }
        else
        {
            if (application.suspended)
            {
                ZZ_LOG_INFO("Window restored. Resuming application.");
                application.suspended = FALSE;
            }
            application.program->resize(application.program, width, height);
            render_resize(width, height);
        }
    }

    return FALSE;
}

b8 application_initialize(struct program* program)
{
    if (application_initialized)
    {
        ZZ_LOG_ERROR("application_initialize called more than once.");
        return FALSE;
    }

    application.program = program;
    application.running = TRUE;
    application.suspended = FALSE;

    struct application_config* config = &(application.program->application_config);
    if (!platform_application_initialize(&application.platform_application, config->name, config->x, config->y, config->width, config->height))
    {
        return FALSE;
    }

    if (!render_initialize(config->name, &application.platform_application))
    {
        ZZ_LOG_FATAL("Failed to initialize render.");
        return FALSE;
    }

    if (!application.program->initialize(application.program))
    {
        ZZ_LOG_FATAL("Failed to initialize program.");
        return FALSE;
    }

    event_register_receiver(ZZ_EVENT_CODE_QUIT, 0, application_on_quit);
    event_register_receiver(ZZ_EVENT_CODE_RESIZE, 0, application_on_resize);
    
    union event_data event_data;
    event_data.u16[0] = (u16)config->width;
    event_data.u16[1] = (u16)config->height;
    event_send(ZZ_EVENT_CODE_RESIZE, 0, event_data);

    application_initialized = TRUE;
    ZZ_LOG_INFO("Application initialized.");
    return TRUE;
}

void application_deinitialize()
{
    application_initialized = FALSE;
    ZZ_LOG_INFO("Application deinitialized.");
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
            f64 frame_start_time = time_get();

            input_update(delta_time);

            if (!application.program->update(application.program, delta_time))
            {
                ZZ_LOG_FATAL("Game update failed.");
                application.running = FALSE;
                break;
            }
            
            if (!application.program->render(application.program, delta_time))
            {
                ZZ_LOG_FATAL("Game render failed.");
                application.running = FALSE;
                break;
            }
            struct render_packet packet;
            packet.delta_time = delta_time;
            render_draw_frame(&packet);

            f64 current_time = time_get();
            f64 frame_elapsed_time = current_time - frame_start_time;
            //running_time += frame_elapsed_time;
            f64 remaining_time = target_time_per_frame - frame_elapsed_time;

            if (remaining_time > 0)
            {
                u64 remaining_time_milliseconds = remaining_time * 1000;
                b8 limit_frames = FALSE;
                if (remaining_time_milliseconds > 0 && limit_frames)
                {
                    time_sleep(remaining_time_milliseconds);
                }

                //frame_count += 1;
            }

            application.last_time = current_time;
        }
    }
    application.running = FALSE;

    event_unregister_receiver(ZZ_EVENT_CODE_QUIT, 0, application_on_quit);
    event_unregister_receiver(ZZ_EVENT_CODE_RESIZE, 0, application_on_resize);

    render_deinitialize();
    platform_application_deinitialize(&application.platform_application);

    return TRUE;
}

void application_get_size(u32* width, u32* height)
{
    *width = application.width;
    *height = application.height;
}