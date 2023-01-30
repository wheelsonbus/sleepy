#pragma once

#include "zz.h"

#include "backend/render.h"

struct program;
struct application;

struct render
{
    struct program* program;
    struct memory* memory;
    struct event* event;
    struct application* application;

    struct backend_render backend_render;
};

struct render_config
{
    struct program* program;
    struct memory* memory;
    struct event* event;
    struct application* application;
};

struct render_packet
{
    f32 delta_time;
};

b8 render_create(struct render* render, struct render_config* config);
void render_destroy(struct render* render);

b8 render_draw_frame(struct render* render, struct render_packet* packet);