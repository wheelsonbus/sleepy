#ifndef ZZ_ENTRY_H
#define ZZ_ENTRY_H

#include "program.h"
#include "log.h"

extern void entry(struct program_config* program_config);

int main()
{
    struct program_config program_config;
    entry(&program_config);
    if (!program_initialize(&program_config))
    {
        ZZ_LOG_FATAL("Failed to create program.");
        return 1;
    }

    if (!program_loop())
    {
        ZZ_LOG_ERROR("program_loop returned ZZ_FALSE.");
        return 2;
    }

    program_deinitialize();

    return 0;
}

#endif