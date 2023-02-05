#include "playground.h"

#include <zz/event.h>

#include "camera.h"
#include "box.h"

static struct camera camera;
static memory_array_box_t boxes;

b8 playground_on_initialize(struct program* program)
{
    camera.position.x = 0.0f;
    camera.position.y = 0.0f;
    camera.position.z = 0.0f;
    camera.clip_left = -4.0f;
    camera.clip_right = 4.0f;
    camera.clip_top = -4.0f;
    camera.clip_bottom = 4.0f;
    camera.clip_near = -1.0f;
    camera.clip_far = 1.0f;

    memory_array_create_and_reserve(&program->memory, &boxes, 2);
    memory_array_push(&program->memory, &boxes, ((struct box){(vec3){-2.0f, 1.0f, 0.5f}, (struct sprite){(vec2){2.0f, 2.0f}}}));
    memory_array_push(&program->memory, &boxes, ((struct box){(vec3){-1.0f, -1.0f, 0.0f}, (struct sprite){(vec2){2.0f, 2.0f}}}));
    memory_array_push(&program->memory, &boxes, ((struct box){(vec3){0.0f, 0.0f, -0.5f}, (struct sprite){(vec2){2.0f, 2.0f}}}));

    return TRUE;
}

b8 playground_on_deinitialize(struct program* program)
{
    memory_array_destroy(&program->memory, &boxes);

    return TRUE;
}

b8 playground_on_tick(struct program* program, u64 delta_time)
{
    f64 m = (float)delta_time * 0.001f * 2.0f;
    camera.position.x -= m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_LEFT);
    camera.position.x += m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_RIGHT);
    camera.position.y -= m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_UP);
    camera.position.y += m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_DOWN);
    camera.position.z -= m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_Z);
    camera.position.z += m * input_get_key_state(&program->input, ZZ_INPUT_KEY_CODE_X);

    program_set_view_matrix(program, camera_get_view_matrix(&camera));
    program_set_projection_matrix(program, camera_get_projection_matrix(&camera));

    return TRUE;
}

b8 playground_on_frame(struct program* program, u64 delta_time)
{
    for (u16 i = 0; i < boxes.length; i += 1)
    {
        box_draw(&boxes.data[i], program);
    }

    return TRUE;
}