#pragma once

#include "memory.h"
#include "event.h"
#include "input.h"
#include "application.h"
#include "render.h"

struct program
{
    struct memory memory;
    struct event event;
    struct input input;
    struct application application;
    struct render render;

    u16 width, height;
    b8 running, suspended;

    u64 accumulated_tick_time;
    u64 accumulated_frame_time;
    u64 last_frame_time;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();

    b8 (*on_tick)(u64 delta_time);
    b8 (*on_frame)(u64 delta_time);
};

struct program_config
{
    char* name;
    u16 x, y;
    u16 width, height;

    b8 (*on_initialize)();
    b8 (*on_deinitialize)();
    
    b8 (*on_tick)(u64 delta_time);
    b8 (*on_frame)(u64 delta_time);
};

ZZ_API b8 program_initialize(struct program_config* config);
ZZ_API void program_deinitialize();

ZZ_API b8 program_loop();

ZZ_API void program_set_model_matrix(mat4 matrix);
ZZ_API void program_set_view_matrix(mat4 matrix);
ZZ_API void program_set_projection_matrix(mat4 matrix);
ZZ_API void program_draw_sprite(struct sprite* sprite, vec3 position);