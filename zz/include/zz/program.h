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

    b8 (*initialize)();
    b8 (*deinitialize)();
};

struct program_config
{
    char* name;
    u16 x, y;
    u16 width, height;

    b8 (*initialize)();
    b8 (*deinitialize)();
    
    b8 (*update)(f64 delta_time);
    b8 (*render)(f64 delta_time);

    void (*resize)(u32 width, u32 height);
};

ZZ_API b8 program_create(struct program* program, struct program_config* config);
ZZ_API void program_destroy(struct program* program);

ZZ_API b8 program_loop(struct program* program);