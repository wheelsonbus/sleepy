#include "camera.h"

mat4 camera_get_view_matrix(struct camera* camera)
{
    f32 multiplier;
    if (camera->position.z == camera->clip_far)
    {
        multiplier = 1.0f;
    }
    else
    {
        multiplier = 1.0f / (camera->clip_far - camera->position.z);
    }
    return (mat4)
    {
        .x = {1.0f, 0.0f, 0.0f, 0.0f},
        .y = {0.0f, 1.0f, 0.0f, 0.0f},
        .z = {camera->position.x * multiplier, camera->position.y * multiplier, 1.0f, 0.0f},
        .w = {-camera->position.x * multiplier, -camera->position.y * multiplier, 0.0f, 1.0f}
    };
}

mat4 camera_get_projection_matrix(struct camera* camera)
{
    return mat4_orthographic(camera->clip_left, camera->clip_right, camera->clip_top, camera->clip_bottom, camera->clip_near, camera->clip_far);
}