#include <entry.h>
#include <core/memory/memory.h>

#include "playground.h"

b8 entry(struct program* program)
{
    program->application_config.x = 100;
    program->application_config.y = 100;
    program->application_config.width = 1280;
    program->application_config.height = 720;
    program->application_config.name = "Sleepy Engine Playground";

    program->initialize = playground_initialize;
    program->update = playground_update;
    program->render = playground_render;

    program->resize = playground_resize;

    program->state = memory_allocate(sizeof(struct playground_state), ZZ_MEMORY_TAG_GAME);

    return TRUE;
}