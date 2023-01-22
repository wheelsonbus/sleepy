#pragma once

#include "core/application/application.h"

struct program
{
    struct application_config application_config;

    b8 (*initialize)(struct program* program);
    b8 (*deinitialize)(struct program* program);
    b8 (*update)(struct program* program, f32 delta_time);
    b8 (*render)(struct program* program, f32 delta_time);

    void (*resize)(struct program* program, u32 width, u32 height);

    void* state;
};