#pragma once

#include "zz/zz.h"

#include "zz/application.h"
#include "zz/sprite.h"

struct backend_render_config
{

};

b8 backend_render_initialize(struct backend_render_config* config);
void backend_render_deinitialize();

void backend_render_resize();

b8 backend_render_draw_frame();

void backend_render_set_model_matrix(mat4 matrix);
void backend_render_set_view_matrix(mat4 matrix);
void backend_render_set_projection_matrix(mat4 matrix);
void backend_render_draw_sprite(struct sprite* sprite, vec3 position);
