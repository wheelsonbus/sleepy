#pragma once

#include "core/application/application.h"
#include "core/logger/logger.h"
#include "core/memory/memory.h"
#include "core/event/event.h"
#include "game.h"

extern b8 entry(struct game* game);

int main()
{
    if (!logger_initialize()) { return -1; }
    if (!memory_initialize()) { return -2; }
    if (!event_initialize()) { return -3; }

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
        return -2;
    }
    if (!game.render)
    {
        ZZ_LOG_FATAL("The game's render function pointer is not assigned.");
        return -2;
    }
    if (!game.resize)
    {
        ZZ_LOG_FATAL("The game's resize function pointer is not assigned.");
        return -2;
    }

    if (!application_initialize(&game))
    {
        ZZ_LOG_FATAL("Application failed to initialize.");
        return 1;
    }
    if (!application_run())
    {
        ZZ_LOG_FATAL("Application did not close gracefully.");
        return 2;
    }

    event_deinitialize();
    memory_deinitialize();
    logger_deinitialize();

    return 0;
}