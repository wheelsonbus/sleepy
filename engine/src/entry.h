#pragma once

#include "program.h"
#include "core/application/application.h"
#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/event/event.h"
#include "core/input/input.h"

extern b8 entry(struct program* program);

int main()
{
    if (!log_initialize()) { return -1; }
    if (!memory_initialize()) { return -2; }
    if (!event_initialize()) { return -3; }
    if (!input_initialize()) { return -4; }

    struct program program;
    if (!entry(&program))
    {
        ZZ_LOG_FATAL("Could not create game.");
        return -1;
    }

    if (!program.initialize)
    {
        ZZ_LOG_FATAL("The program's initialize function pointer is not assigned.");
        return -2;
    }
    if (!program.deinitialize)
    {
        ZZ_LOG_FATAL("The program's deinitialize function pointer is not assigned.");
        return -3;
    }
    if (!program.update)
    {
        ZZ_LOG_FATAL("The program's update function pointer is not assigned.");
        return -4;
    }
    if (!program.render)
    {
        ZZ_LOG_FATAL("The program's render function pointer is not assigned.");
        return -5;
    }
    if (!program.resize)
    {
        ZZ_LOG_FATAL("The program's resize function pointer is not assigned.");
        return -6;
    }

    if (!application_initialize(&program))
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