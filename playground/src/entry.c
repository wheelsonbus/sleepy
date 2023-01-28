#include <zz/entry.h>
#include <zz/memory.h>

#include "playground.h"

void entry(struct program_config* program_config)
{
    program_config->name = "Sleepy Engine Playground";
    program_config->x = 100;
    program_config->y = 100;
    program_config->width = 1280;
    program_config->height = 720;
    program_config->initialize = playground_initialize;
    program_config->deinitialize = playground_deinitialize;
    program_config->update = playground_update;
    program_config->render = playground_render;
    program_config->resize = playground_resize;
}