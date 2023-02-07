#include <zz/entry.h>

#include "playground.h"

void entry(struct client_config* client_config)
{
    client_config->name = "Sleepy Engine Playground";
    client_config->x = 560;
    client_config->y = 80;
    client_config->width = 1280;
    client_config->height = 720;
    client_config->on_initialize = playground_on_initialize;
    client_config->on_deinitialize = playground_on_deinitialize;
    client_config->on_tick = playground_on_tick;
    client_config->on_frame = playground_on_frame;
}