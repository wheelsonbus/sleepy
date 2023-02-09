#include "server.h"

#include <zz/log.h>
#include <zz/event.h>
#include <zz/render.h>
#include <playground/network.h>

#define TIMEOUT_MILLISECONDS 5000

static struct server server;

b8 server_on_initialize()
{
    server.tick = 0;

    zz_memory_array_create(&server.clients);

    return ZZ_TRUE;
}

b8 server_on_deinitialize()
{
    zz_memory_array_destroy(&server.clients);

    return ZZ_TRUE;
}

b8 server_on_tick()
{
    f64 m = (float)server_milliseconds_per_tick * 0.001f * 2.0f;
    for (u16 i = 0; i < server.clients.length; ++i)
    {
        struct network_client_input input;
        b8 input_available = ZZ_FALSE;
        for (u16 j = 0; j < server.clients.data[i].inputs.length; ++j)
        {
            if (server.clients.data[i].inputs.data[j].client_tick == server.tick)
            {
                input = server.clients.data[i].inputs.data[j];
                input_available = ZZ_TRUE;
            }
        }

        if (!input_available)
        {
            input = server.clients.data[i].previous_input;
        }
        
        server.clients.data[i].box.position.x -= m * input.left;
        server.clients.data[i].box.position.x += m * input.right;
        server.clients.data[i].box.position.y -= m * input.up;
        server.clients.data[i].box.position.y += m * input.down;

        if (input_available)
        {
            server.clients.data[i].previous_input = input;
        }
    }

    for (u16 i = 0; i < server.clients.length; ++i)
    {
        struct network_server_state state;
        state.tick = server.tick;
        state.client_state.position = server.clients.data[i].box.position;

        state.position_count = server.clients.length - 1;
        u16 k = 0;
        for (u16 j = 0; j < server.clients.length; ++j)
        {
            if (i != j)
            {
                state.positions[k] = server.clients.data[k].box.position;
                ++k;
            }
        }

        struct zz_network_packet packet;
        packet.size = network_server_message_write_state(packet.buffer, &state);
        packet.ip_endpoint = server.clients.data[i].ip_endpoint;
        zz_network_send(&packet);

        server.clients.data[i].timeout += server_milliseconds_per_tick;
        if (server.clients.data[i].timeout >= TIMEOUT_MILLISECONDS)
        {
            zz_memory_array_pop_at(&server.clients, i);
            ZZ_LOG_INFO("Client timed out.");
        }
    }

    server.tick = (server.tick + 1) % NETWORK_MAX_TICKS_AHEAD;

    return ZZ_TRUE;
}

b8 server_on_packet(struct zz_network_packet* packet)
{
    switch ((enum network_client_message_type)packet->buffer[0])
    {
        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION:
        {
            for (u16 i = 0; i < server.clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&server.clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    return ZZ_TRUE;
                }
            }

            struct client client;
            client.ip_endpoint = packet->ip_endpoint;
            client.timeout = 0;
            zz_memory_array_create_and_reserve(&client.inputs, 1);
            struct network_client_input input;
            zz_memory_zero(&input, sizeof(input));
            zz_memory_array_push(&client.inputs, input);
            zz_memory_zero(&client.previous_input, sizeof(client.previous_input));
            client.box.position = (vec3){0.0f, 0.0f, 0.0f};
            client.box.sprite.size = (vec2){1.0f, 1.0f};
            zz_memory_array_push(&server.clients, client);

            struct network_server_admission admission;
            admission.success = ZZ_TRUE;
            struct zz_network_packet packet;
            packet.size = network_server_message_write_admission(packet.buffer, &admission);
            packet.ip_endpoint = client.ip_endpoint;
            zz_network_send(&packet);

            ZZ_LOG_INFO("Received client connection request.");
        }
        break;
        
        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION:
        {
            for (u16 i = 0; i < server.clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&server.clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    zz_memory_array_destroy(&server.clients.data[i].inputs);
                    zz_memory_array_pop_at(&server.clients, i);
                    break;
                }
            }

            ZZ_LOG_INFO("Received client disconnection notification.");
        }
        break;

        case PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_INPUT:
        {
            for (u16 i = 0; i < server.clients.length; ++i)
            {
                if (zz_network_ip_endpoint_equals(&server.clients.data[i].ip_endpoint, &packet->ip_endpoint))
                {
                    struct network_client_input input;
                    network_client_message_read_input(packet->buffer, &input);
                    zz_memory_array_push(&server.clients.data[i].inputs, input);

                    server.clients.data[i].timeout = 0;

                    break;
                }
            }
        }
        break;
    }

    return ZZ_TRUE;
}