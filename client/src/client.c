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
    client.server_state.tick = NETWORK_MAX_TICKS;
    client.tick = NETWORK_MAX_TICKS;

    client.latency_tick = NETWORK_MAX_TICKS;

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
    if (client.server_state.tick != NETWORK_MAX_TICKS)
    {
        if (client.tick == NETWORK_MAX_TICKS)
        {
            client.network_states[client.server_state.tick].server_state = client.server_state;
            client.tick = (client.server_state.tick + 1) % NETWORK_MAX_TICKS;
        }
        else
        {
            if (client.latency_tick != NETWORK_MAX_TICKS)
            {
                if ((client.server_state.tick - client.latency_tick) % NETWORK_MAX_TICKS > 0 && (client.server_state.tick - client.latency_tick) % NETWORK_MAX_TICKS < NETWORK_MAX_TICKS_AHEAD)
                {
                    if (client.server_state.latency > NETWORK_LATENCY_TARGET + NETWORK_LATENCY_TOLERANCE || client.server_state.latency < NETWORK_LATENCY_TARGET - NETWORK_LATENCY_TOLERANCE)
                    {
                        ZZ_LOG_DEBUG("RESYNC %d", client.server_state.latency);
                        client_go_to_tick((client.tick + client.server_state.latency - NETWORK_LATENCY_TARGET) % NETWORK_MAX_TICKS);
                    }
                    client.latency_tick = client.tick;
                }
            }
            else
            {
                client.latency_tick = client.tick;
            }
            if (!network_client_state_equals(&client.network_states[client.server_state.tick].server_state.client_state, &client.server_state.client_state))
            {
                ZZ_LOG_DEBUG("PANIC x%u", (client.tick - client.server_state.tick) % NETWORK_MAX_TICKS);
                client.network_states[client.server_state.tick].server_state = client.server_state;
                for (u32 i = client.server_state.tick; i != client.tick; i = (i + 1) % NETWORK_MAX_TICKS)
                {
                    client_predict_tick(&client.network_states[i].server_state, &client.network_states[(i + 1) % NETWORK_MAX_TICKS]);
                }
            }
            else
            {
                client.network_states[client.server_state.tick].server_state = client.server_state;
            }

            client.tick = (client.tick + 1) % NETWORK_MAX_TICKS;

            struct network_client_input input;
            input.server_tick = client.server_state.tick;
            input.client_tick = client.tick;
            input.left = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_LEFT);
            input.right = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_RIGHT);
            input.up = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_UP);
            input.down = zz_input_get_key_state(ZZ_INPUT_KEY_CODE_DOWN);

            struct zz_network_packet packet;
            packet.size = network_client_message_write_input(packet.buffer, &input);
            packet.ip_endpoint = zz_client_get_server_ip_endpoint();
            zz_network_send(&packet);

            client.network_states[client.tick].client_input = input;
            client_predict_tick(&client.network_states[(client.tick - 1) % NETWORK_MAX_TICKS].server_state, &client.network_states[client.tick]);

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
    }

    return ZZ_TRUE;
}

b8 client_on_frame(u64 delta_time)
{
    if (client.tick != NETWORK_MAX_TICKS)
    {
        struct box box;
        box.position = client.network_states[client.tick].server_state.client_state.position;
        box.sprite.size = vec2_fill(1.0f, 1.0f);
        box_draw(&box);

        for (u32 i = 0; i < client.server_state.position_count; ++i)
        {
            box.position = client.server_state.positions[i];
            box.sprite.size = vec2_fill(1.0f, 1.0f);
            box_draw(&box);
        }
    }

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
            network_server_message_read_state(packet->buffer, &client.server_state);
        }
        break;
    }

    return ZZ_TRUE;
}

b8 client_go_to_tick(u32 tick)
{
    if ((client.tick - client.server_state.tick) % NETWORK_MAX_TICKS < (tick - client.server_state.tick) % NETWORK_MAX_TICKS)
    {
        for (u32 i = client.tick; i != tick; i = (i + 1) % NETWORK_MAX_TICKS)
        {
            client.network_states[(i + 1) % NETWORK_MAX_TICKS].client_input = client.network_states[client.tick].client_input;
            client_predict_tick(&client.network_states[i].server_state, &client.network_states[(i + 1) % NETWORK_MAX_TICKS]);
        }
    }
    client.tick = tick;

    return ZZ_TRUE;
}

b8 client_predict_tick(const struct network_server_state* state, struct network_state* predicted_state)
{
    predicted_state->server_state = *state;

    f64 m = (float)client_milliseconds_per_tick * 0.001f * 2.0f;
    predicted_state->server_state.client_state.position.x -= m * predicted_state->client_input.left;
    predicted_state->server_state.client_state.position.x += m * predicted_state->client_input.right;
    predicted_state->server_state.client_state.position.y -= m * predicted_state->client_input.up;
    predicted_state->server_state.client_state.position.y += m * predicted_state->client_input.down;

    return ZZ_TRUE;
}

