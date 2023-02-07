#ifndef PLAYGROUND_NETWORK_H
#define PLAYGROUND_NETWORK_H

#include <playground/playground.h>

#include <zz/zz.h>
#include <zz/math.h>

enum network_client_message_type
{
    ZZ_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION,
    ZZ_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION,
    ZZ_NETWORK_CLIENT_MESSAGE_TYPE_INPUT
};

struct network_client_message_connection
{

};

struct network_client_message_disconnection
{

};

enum network_client_input_button_flag
{
    ZZ_NETWORK_CLIENT_INPUT_BUTTON_FLAG_LEFT = 1 << 0,
    ZZ_NETWORK_CLIENT_INPUT_BUTTON_FLAG_RIGHT = 1 << 1,
    ZZ_NETWORK_CLIENT_INPUT_BUTTON_FLAG_UP = 1 << 2,
    ZZ_NETWORK_CLIENT_INPUT_BUTTON_FLAG_DOWN = 1 << 3,
};

struct network_client_message_input
{
    b8 left, right, up, down;
};

PLAYGROUND_API u32 network_client_message_connection_write(u8* buffer, const struct network_client_message_connection* connection);
PLAYGROUND_API void network_client_message_connection_read(u8* buffer, struct network_client_message_connection* connection);
PLAYGROUND_API u32 network_client_message_disconnection_write(u8* buffer, const struct network_client_message_disconnection* disconnection);
PLAYGROUND_API void network_client_message_disconnection_read(u8* buffer, struct network_client_message_disconnection* disconnection);
PLAYGROUND_API u32 network_client_message_input_write(u8* buffer, const struct network_client_message_input* input);
PLAYGROUND_API void network_client_message_input_read(u8* buffer, struct network_client_message_input* input);

enum network_server_message_type
{
    ZZ_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION,
    ZZ_NETWORK_SERVER_MESSAGE_TYPE_STATE
};

struct network_server_message_admission
{
    b8 success;
};

struct network_server_message_state
{
    u8 position_count;
    vec3 positions[255];
};

PLAYGROUND_API u32 network_server_message_admission_write(u8* buffer, const struct network_server_message_admission* admission);
PLAYGROUND_API void network_server_message_admission_read(u8* buffer, struct network_server_message_admission* admission);
PLAYGROUND_API u32 network_server_message_state_write(u8* buffer, const struct network_server_message_state* state);
PLAYGROUND_API void network_server_message_state_read(u8* buffer, struct network_server_message_state* state);

#endif