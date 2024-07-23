#ifndef ZZ_INTERNAL_RENDER_H
#define ZZ_INTERNAL_RENDER_H

#include "zz/zz.h"

#include "zz/application.h"
#include "zz/sprite.h"

struct zz_internal_render_config
{

};

b8 zz_internal_render_initialize(struct zz_internal_render_config* config);
void zz_internal_render_deinitialize();

void zz_internal_render_resize();

b8 zz_internal_render_draw_frame();

void zz_internal_render_set_model_matrix(mat4 matrix);
void zz_internal_render_set_view_matrix(mat4 matrix);
void zz_internal_render_set_projection_matrix(mat4 matrix);
void zz_internal_render_draw_sprite(struct zz_sprite* sprite, vec3 position);

b8 zz_internal_render_manifest_load(char* path);

#endif
