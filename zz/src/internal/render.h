#pragma once

#include "zz/zz.h"

#include "zz/application.h"
#include "zz/sprite.h"

struct internal_render_config
{

};

b8 internal_render_initialize(struct internal_render_config* config);
void internal_render_deinitialize();

void internal_render_resize();

b8 internal_render_draw_frame();

void internal_render_set_model_matrix(mat4 matrix);
void internal_render_set_view_matrix(mat4 matrix);
void internal_render_set_projection_matrix(mat4 matrix);
void internal_render_draw_sprite(struct sprite* sprite, vec3 position);
