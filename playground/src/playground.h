#pragma once

#include <zz/zz.h>
#include <zz/program.h>
#include <zz/log.h>

struct playground_state
{
    f32 delta_time;
};

b8 playground_initialize(struct program* program);
b8 playground_update(struct program* program, f32 delta_time);
b8 playground_render(struct program* program, f32 delta_time);

void playground_resize(struct program* program, u32 width, u32 height);
