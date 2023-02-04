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

    b8 (*on_initialize)(struct program* program);
    b8 (*on_deinitialize)(struct program* program);

    b8 (*on_tick)(struct program* program, f64 delta_time);
    b8 (*on_frame)(struct program* program, f64 delta_time);
};

struct program_config
{
    char* name;
    u16 x, y;
    u16 width, height;

    b8 (*on_initialize)(struct program* program);
    b8 (*on_deinitialize)(struct program* program);
    
    b8 (*on_tick)(struct program* program, f64 delta_time);
    b8 (*on_frame)(struct program* program, f64 delta_time);
};

ZZ_API b8 program_create(struct program* program, struct program_config* config);
ZZ_API void program_destroy(struct program* program);

ZZ_API b8 program_loop(struct program* program);

ZZ_API void program_bind_camera(struct program* program, struct camera* camera);
ZZ_API void program_draw_sprite(struct program* program, struct sprite* sprite, vec3 position);