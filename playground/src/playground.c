#include "playground.h"

#include <zz/log.h>
#include <zz/event.h>
#include <zz/render.h>

#include "camera.h"
#include "box.h"
#include "network.h"

static struct camera camera;
static memory_array_box_t boxes;

b8 playground_on_initialize()
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

    memory_array_create_and_reserve(&boxes, 2);
    memory_array_push(&boxes, ((struct box){(vec3){-2.0f, 1.0f, 0.5f}, (struct sprite){(vec2){2.0f, 2.0f}}}));
    memory_array_push(&boxes, ((struct box){(vec3){-1.0f, -1.0f, 0.0f}, (struct sprite){(vec2){2.0f, 2.0f}}}));
    memory_array_push(&boxes, ((struct box){(vec3){0.0f, 0.0f, -0.5f}, (struct sprite){(vec2){2.0f, 2.0f}}}));

    return ZZ_TRUE;
}

b8 playground_on_deinitialize()
{
    memory_array_destroy(&boxes);

    return ZZ_TRUE;
}

b8 playground_on_tick(u64 delta_time)
{
    f64 m = (float)delta_time * 0.001f * 2.0f;
    camera.position.x -= m * input_get_key_state(ZZ_INPUT_KEY_CODE_LEFT);
    camera.position.x += m * input_get_key_state(ZZ_INPUT_KEY_CODE_RIGHT);
    camera.position.y -= m * input_get_key_state(ZZ_INPUT_KEY_CODE_UP);
    camera.position.y += m * input_get_key_state(ZZ_INPUT_KEY_CODE_DOWN);
    camera.position.z -= m * input_get_key_state(ZZ_INPUT_KEY_CODE_Z);
    camera.position.z += m * input_get_key_state(ZZ_INPUT_KEY_CODE_X);

    render_set_view_matrix(camera_get_view_matrix(&camera));
    render_set_projection_matrix(camera_get_projection_matrix(&camera));

    return ZZ_TRUE;
}

b8 playground_on_frame(u64 delta_time)
{
    for (u16 i = 0; i < boxes.length; i += 1)
    {
        box_draw(&boxes.data[i]);
    }

    return ZZ_TRUE;
}