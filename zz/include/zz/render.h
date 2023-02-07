#ifndef ZZ_RENDER_H
#define ZZ_RENDER_H

#include "zz.h"

#include "zz/internal/render.h"

#include "sprite.h"

struct zz_render
{

};

struct zz_render_config
{

};

b8 zz_render_initialize(struct zz_render_config* config);
void zz_render_deinitialize();

b8 zz_render_draw_frame();

ZZ_API void zz_render_set_model_matrix(mat4 matrix);
ZZ_API void zz_render_set_view_matrix(mat4 matrix);
ZZ_API void zz_render_set_projection_matrix(mat4 matrix);
ZZ_API void zz_render_draw_sprite(struct zz_sprite* sprite, vec3 position);

#endif