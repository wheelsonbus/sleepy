#include "client.h"

#include <zz/log.h>
#include <zz/event.h>
#include <zz/render.h>
#include <zz/network.h>

#include <playground/camera.h>
#include <playground/box.h>
#include <playground/network.h>

static struct camera camera;
static zz_memory_array_box_t boxes;

b8 client_on_initialize()
{
    struct network_client_message_connection connection;
    struct zz_network_packet packet;
    packet.size = network_client_message_connection_write(packet.buffer, &connection);
    packet.ip_endpoint = zz_client_get_server_ip_endpoint();
    zz_network_send(&packet);

    camera.position.x = 0.0f;
    camera.position.y = 0.0f;
    camera.position.z = 0.0f;
    camera.clip_left = -4.0f;
    camera.clip_right = 4.0f;
    camera.clip_top = -4.0f;
    camera.clip_bottom = 4.0f;
    camera.clip_near = -1.0f;
    camera.clip_far = 1.0f;

    zz_memory_array_create(&boxes);

    return ZZ_TRUE;
}

b8 client_on_deinitialize()
{
    zz_memory_array_destroy(&boxes);

    struct network_client_message_disconnection disconnection;
    struct zz_network_packet packet;
    packet.size = network_client_message_disconnection_write(packet.buffer, &disconnection);
    packet.ip_endpoint = zz_client_get_server_ip_endpoint();
    zz_network_send(&packet);

    return ZZ_TRUE;
}

b8 client_on_tick(u64 delta_time)
{
    struct network_client_message_input input;
    input.left = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_LEFT);
    input.right = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_RIGHT);
    input.up = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_UP);
    input.down = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_DOWN);
    struct zz_network_packet packet;
    packet.size = network_client_message_input_write(packet.buffer, &input);
    packet.ip_endpoint = zz_client_get_server_ip_endpoint();
    zz_network_send(&packet);

    f64 m = (float)delta_time * 0.001f * 2.0f;
    camera.position.x -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_A);
    camera.position.x += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_D);
    camera.position.y -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_W);
    camera.position.y += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_S);
    camera.position.z -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_Z);
    camera.position.z += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_X);

    zz_render_set_view_matrix(camera_get_view_matrix(&camera));
    zz_render_set_projection_matrix(camera_get_projection_matrix(&camera));

    return ZZ_TRUE;
}

b8 client_on_frame(u64 delta_time)
{
    for (u16 i = 0; i < boxes.length; i += 1)
    {
        box_draw(&boxes.data[i]);
    }

    return ZZ_TRUE;
}

b8 client_on_packet(struct zz_network_packet* packet)
{
    switch ((enum network_server_message_type)packet->buffer[0])
    {
        case PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION:
        {
            ZZ_LOG_DEBUG("Received server connection admission.");

            break;
        }
        case PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_STATE:
        {
            struct network_server_message_state state;
            network_server_message_state_read(packet->buffer, &state);

            zz_memory_array_clear(&boxes);
            for (u16 i = 0; i < state.position_count; ++i)
            {
                struct box box;
                box.position = state.positions[i];
                box.sprite.size = (vec2){1.0f, 1.0f};
                zz_memory_array_push(&boxes, box);
            }

            break;
        }
    }

    return ZZ_TRUE;
}