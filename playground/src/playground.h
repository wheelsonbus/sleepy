#pragma once

#include <zz.h>
#include <game.h>
#include <core/log/log.h>

struct playground_state
{
    f32 delta_time;
};

b8 playground_initialize(struct game* game);
b8 playground_update(struct game* game, f32 delta_time);
b8 playground_render(struct game* game, f32 delta_time);

void playground_resize(struct game* game, u32 width, u32 height);
