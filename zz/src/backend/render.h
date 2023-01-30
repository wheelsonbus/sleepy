#pragma once

#include "zz/zz.h"

#include "zz/application.h"

#include "vulkan/render.h"

struct backend_render;

struct backend_render_config
{
    struct application* application;
};

b8 backend_render_create(struct backend_render* backend_render, struct backend_render_config* config);
void backend_render_destroy(struct backend_render* backend_render);