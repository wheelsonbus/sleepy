#include <entry.h>
#include <core/memory/memory.h>

#include "playground.h"

b8 entry(struct game* game)
{
    game->application_config.x = 100;
    game->application_config.y = 100;
    game->application_config.width = 1280;
    game->application_config.height = 720;
    game->application_config.name = "Sleepy Engine Playground";

    game->initialize = playground_initialize;
    game->update = playground_update;
    game->render = playground_render;

    game->resize = playground_resize;

    game->state = memory_allocate(sizeof(struct playground_state), ZZ_MEMORY_TAG_GAME);

    return TRUE;
}