#include "playground/network.h"

#include <zz/assert.h>
#include <zz/memory.h>

static void write_u8(u8** buffer_iterator, u8 u)
{
    **buffer_iterator = u;
    ++(*buffer_iterator);
}
/*
static void write_u16(u8** buffer_iterator, u16 u)
{
    zz_memory_copy(*buffer_iterator, &u, sizeof(u));
    *buffer_iterator += sizeof(u);
}
*/
static void write_u32(u8** buffer_iterator, u32 u)
{
    zz_memory_copy(*buffer_iterator, &u, sizeof(u));
    *buffer_iterator += sizeof(u);
}
/*
static void write_u64(u8** buffer_iterator, u64 u)
{
    zz_memory_copy(*buffer_iterator, &u, sizeof(u));
    *buffer_iterator += sizeof(u);
}
*/
static void write_f32(u8** buffer_iterator, f32 f)
{
    zz_memory_copy(*buffer_iterator, &f, sizeof(f));
    *buffer_iterator += sizeof(f);
}
/*
static void write_f64(u8** buffer_iterator, f64 f)
{
    zz_memory_copy(*buffer_iterator, &f, sizeof(f));
    *buffer_iterator += sizeof(f);
}

static void write_vec2(u8** buffer_iterator, vec2 v)
{
    write_f32(buffer_iterator, v.x);
    write_f32(buffer_iterator, v.y);
}
*/
static void write_vec3(u8** buffer_iterator, vec3 v)
{
    write_f32(buffer_iterator, v.x);
    write_f32(buffer_iterator, v.y);
    write_f32(buffer_iterator, v.z);
}

static void read_u8(u8** buffer_iterator, u8* u)
{
	*u = **buffer_iterator;
	++(*buffer_iterator);
}
/*
static void read_u16(u8** buffer_iterator, u16* u)
{
    zz_memory_copy(u, *buffer_iterator, sizeof(*u));
    *buffer_iterator += sizeof(*u);
}
*/
static void read_u32(u8** buffer_iterator, u32* u)
{
    zz_memory_copy(u, *buffer_iterator, sizeof(*u));
    *buffer_iterator += sizeof(*u);
}
/*
static void read_u64(u8** buffer_iterator, u64* u)
{
    zz_memory_copy(u, *buffer_iterator, sizeof(*u));
    *buffer_iterator += sizeof(*u);
}
*/
static void read_f32(u8** buffer_iterator, f32* f)
{
    zz_memory_copy(f, *buffer_iterator, sizeof(*f));
    *buffer_iterator += sizeof(*f);
}
/*
static void read_f64(u8** buffer_iterator, f64* f)
{
    zz_memory_copy(f, *buffer_iterator, sizeof(*f));
    *buffer_iterator += sizeof(*f);
}

static void read_vec2(u8** buffer_iterator, vec2* v)
{
    read_f32(buffer_iterator, &v->x);
    read_f32(buffer_iterator, &v->y);
}
*/
static void read_vec3(u8** buffer_iterator, vec3* v)
{
    read_f32(buffer_iterator, &v->x);
    read_f32(buffer_iterator, &v->y);
    read_f32(buffer_iterator, &v->z);
}

b8 network_client_state_equals(const struct network_client_state* a, const struct network_client_state* b)
{
    return vec3_equals(a->position, b->position);
}

u32 network_client_message_write_connection(u8* buffer, const struct network_client_connection* connection)
{
    u8* buffer_iterator = buffer;
    write_u8(&buffer_iterator, (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION);

    return (u32)(buffer_iterator - buffer);
}

void network_client_message_read_connection(u8* buffer, struct network_client_connection* connection)
{
    u8* buffer_iterator = buffer;

    u8 message_type;
    read_u8(&buffer_iterator, &message_type);
    ZZ_ASSERT(message_type == (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_CONNECTION);
}

u32 network_client_message_write_disconnection(u8* buffer, const struct network_client_disconnection* disconnection)
{
    u8* buffer_iterator = buffer;
    write_u8(&buffer_iterator, (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION);

    return (u32)(buffer_iterator - buffer);
}

void network_client_message_read_disconnection(u8* buffer, struct network_client_disconnection* disconnection)
{
    u8* buffer_iterator = buffer;

    u8 message_type;
    read_u8(&buffer_iterator, &message_type);
    ZZ_ASSERT(message_type == (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_DISCONNECTION);
}

u32 network_client_message_write_input(u8* buffer, const struct network_client_input* input)
{
    u8* buffer_iterator = buffer;
    write_u8(&buffer_iterator, (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_INPUT);

    write_u32(&buffer_iterator, input->server_tick);
    write_u32(&buffer_iterator, input->client_tick);

    u8 packed_buttons = (u8)(input->left ? PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_LEFT : 0) | (u8)(input->right ? PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_RIGHT : 0) | (u8)(input->up ? PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_UP : 0) | (u8)(input->down ? PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_DOWN : 0);
    write_u8(&buffer_iterator, packed_buttons);

    return (u32)(buffer_iterator - buffer);
}

void network_client_message_read_input(u8* buffer, struct network_client_input* input)
{
    u8* buffer_iterator = buffer;

    u8 message_type;
    read_u8(&buffer_iterator, &message_type);
    ZZ_ASSERT(message_type == (u8)PLAYGROUND_NETWORK_CLIENT_MESSAGE_TYPE_INPUT);

    read_u32(&buffer_iterator, &input->server_tick);
    read_u32(&buffer_iterator, &input->client_tick);

    u8 packed_buttons;
    read_u8(&buffer_iterator, &packed_buttons);
    input->left = packed_buttons & PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_LEFT ? ZZ_TRUE : ZZ_FALSE;
    input->right = packed_buttons & PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_RIGHT ? ZZ_TRUE : ZZ_FALSE;
    input->up = packed_buttons & PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_UP ? ZZ_TRUE : ZZ_FALSE;
    input->down = packed_buttons & PLAYGROUND_NETWORK_CLIENT_INPUT_BUTTON_FLAG_DOWN ? ZZ_TRUE : ZZ_FALSE;
}

u32 network_server_message_write_admission(u8* buffer, const struct network_server_admission* admission)
{
    u8* buffer_iterator = buffer;
    write_u8(&buffer_iterator, (u8)PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION);
    
    write_u8(&buffer_iterator, (u8)admission->success);

    return (u32)(buffer_iterator - buffer);
}

void network_server_message_read_admission(u8* buffer, struct network_server_admission* admission)
{
    u8* buffer_iterator = buffer;

    u8 message_type;
    read_u8(&buffer_iterator, &message_type);
    ZZ_ASSERT(message_type == (u8)PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_ADMISSION);

    u8 success_u8;
    read_u8(&buffer_iterator, &success_u8);
    admission->success = (b8)success_u8;
}

u32 network_server_message_write_state(u8* buffer, const struct network_server_state* state)
{
    u8* buffer_iterator = buffer;
    write_u8(&buffer_iterator, (u8)PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_STATE);
    
    write_u32(&buffer_iterator, state->tick);

    write_u8(&buffer_iterator, state->position_count);
    for (u8 i = 0; i < state->position_count; ++i)
    {
        write_vec3(&buffer_iterator, state->positions[i]);
    }

    write_vec3(&buffer_iterator, state->client_state.position);

    return (u32)(buffer_iterator - buffer);
}

void network_server_message_read_state(u8* buffer, struct network_server_state* state)
{
    u8* buffer_iterator = buffer;

    u8 message_type;
    read_u8(&buffer_iterator, &message_type);
    ZZ_ASSERT(message_type == (u8)PLAYGROUND_NETWORK_SERVER_MESSAGE_TYPE_STATE);
    
    read_u32(&buffer_iterator, &state->tick);

    read_u8(&buffer_iterator, &state->position_count);
    for (u8 i = 0; i < state->position_count; ++i)
    {
        read_vec3(&buffer_iterator, &state->positions[i]);
    }

    read_vec3(&buffer_iterator, &state->client_state.position);
}
