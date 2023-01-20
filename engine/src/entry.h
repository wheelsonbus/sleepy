#pragma once

#include "core/application/application.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/event/event.h"
#include "core/input/input.h"
#include "game.h"

extern b8 entry(struct game* game);

int main()
{
    if (!log_initialize()) { return -1; }
    if (!memory_initialize()) { return -2; }
    if (!event_initialize()) { return -3; }
    if (!input_initialize()) { return -4; }

    struct game game;
    if (!entry(&game))
    {
        ZZ_LOG_FATAL("Could not create game.");
        return -1;
    }

    if (!game.initialize)
    {
        ZZ_LOG_FATAL("The game's initialize function pointer is not assigned.");
        return -2;
    }
    if (!game.update)
    {
        ZZ_LOG_FATAL("The game's update function pointer is not assigned.");
        return -3;
    }
    if (!game.render)
    {
        ZZ_LOG_FATAL("The game's render function pointer is not assigned.");
        return -4;
    }
    if (!game.resize)
    {
        ZZ_LOG_FATAL("The game's resize function pointer is not assigned.");
        return -5;
    }

    if (!application_initialize(&game))
    {
        ZZ_LOG_FATAL("Failed to initialize application.");
        return 1;
    }
    if (!application_run())
    {
        ZZ_LOG_FATAL("Application did not close gracefully.");
        return 2;
    }

    input_deinitialize();
    event_deinitialize();
    memory_deinitialize();
    log_deinitialize();

    return 0;
}