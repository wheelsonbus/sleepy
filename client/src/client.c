#include "client.h"

#include <zz/log.h>
#include <zz/event.h>
#include <zz/render.h>
#include <zz/network.h>

#include <playground/camera.h>
#include <playground/box.h>
#include <playground/network.h>

static struct camera camera;
static struct client client;

b8 client_on_initialize()
{
    client.server_tick = NETWORK_MAX_TICKS_AHEAD;
    client.client_tick = NETWORK_MAX_TICKS_AHEAD;

    struct network_client_connection connection;
    struct zz_network_packet packet;
    packet.size = network_client_message_write_connection(packet.buffer, &connection);
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

    return ZZ_TRUE;
}

b8 client_on_deinitialize()
{
    struct network_client_disconnection disconnection;
    struct zz_network_packet packet;
    packet.size = network_client_message_write_disconnection(packet.buffer, &disconnection);
    packet.ip_endpoint = zz_client_get_server_ip_endpoint();
    zz_network_send(&packet);

    return ZZ_TRUE;
}

b8 client_on_tick()
{
    if (client.client_tick != NETWORK_MAX_TICKS_AHEAD)
    {
        client.client_tick = (client.client_tick + 1) % NETWORK_MAX_TICKS_AHEAD;

        struct network_client_input input;
        input.server_tick = client.server_tick;
        input.client_tick = client.client_tick;
        input.left = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_LEFT);
        input.right = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_RIGHT);
        input.up = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_UP);
        input.down = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_DOWN);

        struct zz_network_packet packet;
        packet.size = network_client_message_write_input(packet.buffer, &input);
        packet.ip_endpoint = zz_client_get_server_ip_endpoint();
        zz_network_send(&packet);

        client.network_states[client.client_tick].client_input = input;
        client_predict_tick(&client.network_states[(client.client_tick - 1) % NETWORK_MAX_TICKS_AHEAD].server_state, &client.network_states[client.client_tick].client_input, &client.network_states[client.client_tick].server_state);

        f64 m = (float)client_milliseconds_per_tick * 0.001f * 2.0f;
        camera.position.x -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_A);
        camera.position.x += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_D);
        camera.position.y -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_W);
        camera.position.y += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_S);
        camera.position.z -= m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_Z);
        camera.position.z += m * zz_input_get_key_state(ZZ_INPUT_KEY_CODE_X);

        zz_render_set_view_matrix(camera_get_view_matrix(&camera));
        zz_render_set_projection_matrix(camera_get_projection_matrix(&camera));
    }

    return ZZ_TRUE;
}

b8 client_on_frame(u64 delta_time)
{
    struct box box;
    box.position = client.network_states[client.client_tick].server_state.client_state.position;
    box.sprite.size = vec2_fill(1.0f, 1.0f);
    box_draw(&box);

    return ZZ_TRUE;
}

b8 client_on_packet(struct zz_network_packet* packet)
{
    switch ((enum network_server_message_type)packet->buffer[0])
    {
        case PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION:
        {
            struct network_server_admission admission;
            network_server_message_read_admission(packet->buffer, &admission);
            if (admission.success)
            {
                ZZ_LOG_INFO("Received server connection admission.");
            }
            else
            {
                ZZ_LOG_INFO("Received server connection rejection.");
            }
        }
        break;

        case PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_STATE:
        {
            struct network_server_state server_state;
            network_server_message_read_state(packet->buffer, &server_state);

            client.server_tick = server_state.tick;
            if (client.client_tick != NETWORK_MAX_TICKS_AHEAD)
            {
                if (!network_client_state_equals(&client.network_states[client.server_tick].server_state.client_state, &server_state.client_state))
                {
                    ZZ_LOG_DEBUG("DESYNCED x%u", client.client_tick - client.server_tick);
                    client.network_states[client.server_tick].server_state = server_state;
                    for (u32 i = client.server_tick; i != client.client_tick; i = (i + 1) % NETWORK_MAX_TICKS_AHEAD)
                    {
                        client_predict_tick(&client.network_states[i].server_state, &client.network_states[(i + 1) % NETWORK_MAX_TICKS_AHEAD].client_input, &client.network_states[(i + 1) % NETWORK_MAX_TICKS_AHEAD].server_state);
                    }
                }
                else
                {
                    client.network_states[client.server_tick].server_state = server_state;
                }
            }
            else
            {
                client.network_states[client.server_tick].server_state = server_state;
                client.client_tick = client.server_tick;
            }
        }
        break;
    }

    return ZZ_TRUE;
}

b8 client_predict_tick(const struct network_server_state* state, const struct network_client_input* input, struct network_server_state* predicted_state)
{
    *predicted_state = *state;

    f64 m = (float)client_milliseconds_per_tick * 0.001f * 2.0f;
    predicted_state->client_state.position.x -= m * input->left;
    predicted_state->client_state.position.x += m * input->right;
    predicted_state->client_state.position.y -= m * input->up;
    predicted_state->client_state.position.y += m * input->down;

    return ZZ_TRUE;
}

