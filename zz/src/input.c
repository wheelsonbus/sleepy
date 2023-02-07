#include "zz/input.h"

#include "zz/log.h"

static struct zz_input input;

b8 zz_input_initialize(struct zz_input_config* config)
{
    zz_memory_zero(&input.state, sizeof(input.state));
    zz_memory_zero(&input.previous_state, sizeof(input.previous_state));

    ZZ_LOG_INFO("Input module initialized.");
    return ZZ_TRUE;
}

void zz_input_deinitialize()
{
    ZZ_LOG_INFO("Input module deinitialized.");
}

void zz_input_update()
{
    zz_memory_copy(&input.previous_state, &input.state, sizeof(input.previous_state));
}

b8 zz_input_get_key_state(enum zz_input_key_code code)
{
    return input.state.key_states[code];
}

b8 zz_input_get_previous_key_state(enum zz_input_key_code code)
{
    return input.previous_state.key_states[code];
}

void zz_input_set_key_state(enum zz_input_key_code code, b8 down)
{
    if (input.state.key_states[code] != down)
    {
        input.state.key_states[code] = down;

        union zz_event_data event_data;
        event_data.u16[0] = (u16)code;
        zz_event_send(down ? ZZ_EVENT_CODE_KEY_PRESS : ZZ_EVENT_CODE_KEY_RELEASE, 0, event_data);
    }
}

b8 zz_input_get_mouse_button_state(enum zz_input_mouse_button_code code)
{
    return input.state.mouse_button_states[code];
}

b8 zz_input_get_previous_mouse_button_state(enum zz_input_mouse_button_code code)
{
    return input.previous_state.mouse_button_states[code];
}

void zz_input_set_mouse_button_state(enum zz_input_mouse_button_code code, b8 down)
{
    if (input.state.mouse_button_states[code] != down)
    {
        input.state.mouse_button_states[code] = down;

        union zz_event_data event_data;
        event_data.u16[0] = code;
        zz_event_send(down ? ZZ_EVENT_CODE_MOUSE_BUTTON_PRESS : ZZ_EVENT_CODE_MOUSE_BUTTON_RELEASE, 0, event_data);
    }
}

void zz_input_get_mouse_position(i32* x, i32* y)
{
    *x = input.state.mouse_x;
    *y = input.state.mouse_y;
}

void zz_input_get_previous_mouse_position(i32* x, i32* y)
{
    *x = input.previous_state.mouse_x;
    *y = input.previous_state.mouse_y;
}

void zz_input_set_mouse_position(i16 x, i16 y)
{
    if (input.state.mouse_x != x || input.state.mouse_y != y)
    {
        input.state.mouse_x = x;
        input.state.mouse_y = y;

        union zz_event_data event_data;
        event_data.u16[0] = x;
        event_data.u16[1] = y;
        zz_event_send(ZZ_EVENT_CODE_MOUSE_MOVE, 0, event_data);
    }
}

void zz_input_move_mouse_wheel(i8 delta)
{
        union zz_event_data event_data;
        event_data.u8[0] = delta;
        zz_event_send(ZZ_EVENT_CODE_MOUSE_WHEEL_MOVE, 0, event_data);
}