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

    b8 (*on_initialize)(struct program* program);
    b8 (*on_deinitialize)(struct program* program);

    b8 (*on_tick)(struct program* program, u64 delta_time);
    b8 (*on_frame)(struct program* program, u64 delta_time);
};

struct program_config
{
    char* name;
    u16 x, y;
    u16 width, height;

    b8 (*on_initialize)(struct program* program);
    b8 (*on_deinitialize)(struct program* program);
    
    b8 (*on_tick)(struct program* program, u64 delta_time);
    b8 (*on_frame)(struct program* program, u64 delta_time);
};

ZZ_API b8 program_create(struct program* program, struct program_config* config);
ZZ_API void program_destroy(struct program* program);

ZZ_API b8 program_loop(struct program* program);

ZZ_API void program_set_model_matrix(struct program* program, mat4 matrix);
ZZ_API void program_set_view_matrix(struct program* program, mat4 matrix);
ZZ_API void program_set_projection_matrix(struct program* program, mat4 matrix);
ZZ_API void program_draw_sprite(struct program* program, struct sprite* sprite, vec3 position);