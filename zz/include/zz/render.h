#pragma once

#include "zz.h"

#include "internal/render.h"

#include "sprite.h"

struct render
{

};

struct render_config
{

};

b8 render_initialize(struct render_config* config);
void render_deinitialize();

b8 render_draw_frame();

void render_set_model_matrix(mat4 matrix);
void render_set_view_matrix(mat4 matrix);
void render_set_projection_matrix(mat4 matrix);
void render_draw_sprite(struct sprite* sprite, vec3 position);