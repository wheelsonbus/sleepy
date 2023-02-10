#ifndef PLAYGROUND_NETWORK_H
#define PLAYGROUND_NETWORK_H

#include <playground/playground.h>

#include <zz/zz.h>
#include <zz/math.h>

#define NETWORK_MILLISECONDS_PER_TICK 10

#define NETWORK_MAX_TICKS 256
#define NETWORK_MAX_TICKS_AHEAD 32

#define NETWORK_LATENCY_TARGET -2
#define NETWORK_LATENCY_TOLERANCE 1

enum network_client_message_type
{
    PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION,
    PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION,
    PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_INPUT
};

struct network_client_connection
{

};

struct network_client_disconnection
{

};

enum network_client_input_button_flag
{
    PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_LEFT = 1 << 0,
    PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_RIGHT = 1 << 1,
    PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_UP = 1 << 2,
    PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_DOWN = 1 << 3,
};

struct network_client_input
{
    u32 server_tick;
    u32 client_tick;
    b8 left, right, up, down;
};

struct network_client_state
{
    vec3 position;
};

typedef struct {struct network_client_input* data; u16 length, capacity;} zz_memory_array_network_client_input_t;

PLAYGROUND_API b8 network_client_state_equals(const struct network_client_state* a, const struct network_client_state* b);

PLAYGROUND_API u32 network_client_message_write_connection(u8* buffer, const struct network_client_connection* connection);
PLAYGROUND_API void network_client_message_read_connection(u8* buffer, struct network_client_connection* connection);
PLAYGROUND_API u32 network_client_message_write_disconnection(u8* buffer, const struct network_client_disconnection* disconnection);
PLAYGROUND_API void network_client_message_read_disconnection(u8* buffer, struct network_client_disconnection* disconnection);
PLAYGROUND_API u32 network_client_message_write_input(u8* buffer, const struct network_client_input* input);
PLAYGROUND_API void network_client_message_read_input(u8* buffer, struct network_client_input* input);

enum network_server_message_type
{
    PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION,
    PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_STATE
};

struct network_server_admission
{
    b8 success;
};

struct network_server_state
{
    u32 tick;
    i32 latency;
    u8 position_count;
    vec3 positions[255];
    struct network_client_state client_state;
};
typedef struct {struct network_server_state* data; u16 length, capacity;} zz_memory_array_network_server_state_t;

PLAYGROUND_API u32 network_server_message_write_admission(u8* buffer, const struct network_server_admission* admission);
PLAYGROUND_API void network_server_message_read_admission(u8* buffer, struct network_server_admission* admission);
PLAYGROUND_API u32 network_server_message_write_state(u8* buffer, const struct network_server_state* state);
PLAYGROUND_API void network_server_message_read_state(u8* buffer, struct network_server_state* state);

struct network_state
{
    struct network_server_state server_state;
    struct network_client_input client_input;
};
typedef struct {struct network_state* data; u16 length, capacity;} zz_memory_array_network_state_t;

#endif