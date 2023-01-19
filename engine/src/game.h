#pragma once

#include "core/application/application.h"

struct game
{
    struct application_config application_config;

    b8 (*initialize)(struct game* game);
    b8 (*update)(struct game* game, f32 delta_time);
    b8 (*render)(struct game* game, f32 delta_time);

    void (*resize)(struct game* game, u32 width, u32 height);

    void* state;
};