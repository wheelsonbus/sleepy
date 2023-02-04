#include <zz/entry.h>
#include <zz/memory.h>

#include "playground.h"

void entry(struct program_config* program_config)
{
    program_config->name = "Sleepy Engine Playground";
    program_config->x = 560;
    program_config->y = 80;
    program_config->width = 1280;
    program_config->height = 720;
    program_config->on_initialize = playground_on_initialize;
    program_config->on_deinitialize = playground_on_deinitialize;
    program_config->on_tick = playground_on_tick;
    program_config->on_frame = playground_on_frame;
}