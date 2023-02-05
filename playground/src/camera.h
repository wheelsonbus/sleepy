#pragma once

#include <zz/zz.h>
#include <zz/math.h>

struct camera
{
    vec3 position;
    f32 clip_left, clip_right, clip_top, clip_bottom, clip_near, clip_far;
};

mat4 camera_get_view_matrix(struct camera* camera);
mat4 camera_get_projection_matrix(struct camera* camera);