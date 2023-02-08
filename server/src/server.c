#include "server.h"

#include <zz/log.h>
#include <zz/event.h>
#include <zz/render.h>
#include <playground/network.h>

#define TIMEOUT_MILLISECONDS 5000

static zz_memory_array_client_t clients;

b8 server_on_initialize()
{
    zz_memory_array_create(&clients);

    return ZZ_TRUE;
}

b8 server_on_deinitialize()
{
    zz_memory_array_destroy(&clients);

    return ZZ_TRUE;
}

b8 server_on_tick(u64 delta_time)
{
    f64 m = (float)delta_time * 0.001f * 2.0f;
    for (u16 i = 0; i < clients.length; ++i)
    {
        clients.data[i].box.position.x -= m * clients.data[i].input_state.left;
        clients.data[i].box.position.x += m * clients.data[i].input_state.right;
        clients.data[i].box.position.y -= m * clients.data[i].input_state.up;
        clients.data[i].box.position.y += m * clients.data[i].input_state.down;
    }

    struct network_server_message_state state;
    state.position_count = clients.length;
    for (u16 i = 0; i < state.position_count; ++i)
    {
        state.positions[i] = clients.data[i].box.position;
    }
    
    for (u16 i = 0; i < clients.length; ++i)
    {
        struct zz_network_packet packet;
        packet.size = network_server_message_state_write(packet.buffer, &state);
        packet.ip_endpoint = clients.data[i].ip_endpoint;
        zz_network_send(&packet);

        zz_memory_copy(&clients.data[i].previous_input_state, &clients.data[i].input_state, sizeof(clients.data[i].input_state));
        clients.data[i].timeout += delta_time;
        if (clients.data[i].timeout >= TIMEOUT_MILLISECONDS)
        {
            zz_memory_array_pop_at(&clients, i);
        }
    }

    return ZZ_TRUE;
}

b8 server_on_packet(struct zz_network_packet* packet)
{
    switch ((enum network_client_message_type)packet->buffer[0])
    {
        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION:
        {
            for (u16 i = 0; i < clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    return ZZ_TRUE;
                }
            }

            struct client client;
            client.ip_endpoint = packet->ip_endpoint;
            client.timeout = 0;
            zz_memory_zero(&client.input_state, sizeof(client.input_state));
            zz_memory_zero(&client.previous_input_state, sizeof(client.previous_input_state));
            client.box.position = (vec3){0.0f, 0.0f, 0.0f};
            client.box.sprite.size = (vec2){1.0f, 1.0f};
            zz_memory_array_push(&clients, client);

            struct network_server_message_admission admission;
            admission.success = ZZ_TRUE;
            struct zz_network_packet packet;
            packet.size = network_server_message_admission_write(packet.buffer, &admission);
            packet.ip_endpoint = client.ip_endpoint;
            ZZ_LOG_DEBUG("%u", client.ip_endpoint.port);
            zz_network_send(&packet);

            ZZ_LOG_DEBUG("Received client connection request.");
            break;
        }
        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION:
        {
            for (u16 i = 0; i < clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    zz_memory_array_pop_at(&clients, i);
                    break;
                }
            }

            ZZ_LOG_DEBUG("Received client disconnection notification.");
            break;
        }
        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_INPUT:
        {
            for (u16 i = 0; i < clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    struct network_client_message_input input;
                    network_client_message_input_read(packet->buffer, &input);

                    clients.data[i].input_state.left = input.left;
                    clients.data[i].input_state.right = input.right;
                    clients.data[i].input_state.up = input.up;
                    clients.data[i].input_state.down = input.down;

                    clients.data[i].timeout = 0;

                    break;
                }
            }

            break;
        }
    }

    return ZZ_TRUE;
}