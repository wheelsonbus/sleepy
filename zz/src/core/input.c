#include "zz/input.h"

#include "zz/log.h"

b8 input_create(struct input* input, struct input_config* config)
{
    input->memory = config->memory;
    input->event = config->event;

    memory_zero(input->memory, &input->state, sizeof(input->state));
    memory_zero(input->memory, &input->previous_state, sizeof(input->previous_state));

    ZZ_LOG_INFO("Input module created.");
    return TRUE;
}

void input_destroy(struct input* input)
{
    ZZ_LOG_INFO("Input module destroyed.");
}

void input_update(struct input* input, f64 delta_time)
{
    memory_copy(input->memory, &input->previous_state, &input->state, sizeof(input->previous_state));
}

b8 input_get_key_state(struct input* input, enum input_key_code code)
{
    return input->state.key_states[code];
}

b8 input_get_previous_key_state(struct input* input, enum input_key_code code)
{
    return input->previous_state.key_states[code];
}

void input_set_key_state(struct input* input, enum input_key_code code, b8 down)
{
    if (input->state.key_states[code] != down)
    {
        input->state.key_states[code] = down;

        union event_data event_data;
        event_data.u16[0] = (u16)code;
        event_send(input->event, down ? ZZ_EVENT_CODE_KEY_PRESS : ZZ_EVENT_CODE_KEY_RELEASE, 0, event_data);
    }
}

b8 input_get_mouse_button_state(struct input* input, enum input_mouse_button_code code)
{
    return input->state.mouse_button_states[code];
}

b8 input_get_previous_mouse_button_state(struct input* input, enum input_mouse_button_code code)
{
    return input->previous_state.mouse_button_states[code];
}

void input_set_mouse_button_state(struct input* input, enum input_mouse_button_code code, b8 down)
{
    if (input->state.mouse_button_states[code] != down)
    {
        input->state.mouse_button_states[code] = down;

        union event_data event_data;
        event_data.u16[0] = code;
        event_send(input->event, down ? ZZ_EVENT_CODE_MOUSE_BUTTON_PRESS : ZZ_EVENT_CODE_MOUSE_BUTTON_RELEASE, 0, event_data);
    }
}

void input_get_mouse_position(struct input* input, i32* x, i32* y)
{
    *x = input->state.mouse_x;
    *y = input->state.mouse_y;
}

void input_get_previous_mouse_position(struct input* input, i32* x, i32* y)
{
    *x = input->previous_state.mouse_x;
    *y = input->previous_state.mouse_y;
}

void input_set_mouse_position(struct input* input, i16 x, i16 y)
{
    if (input->state.mouse_x != x || input->state.mouse_y != y)
    {
        input->state.mouse_x = x;
        input->state.mouse_y = y;

        union event_data event_data;
        event_data.u16[0] = x;
        event_data.u16[1] = y;
        event_send(input->event, ZZ_EVENT_CODE_MOUSE_MOVE, 0, event_data);
    }
}

void input_move_mouse_wheel(struct input* input, i8 delta)
{
        union event_data event_data;
        event_data.u8[0] = delta;
        event_send(input->event, ZZ_EVENT_CODE_MOUSE_WHEEL_MOVE, 0, event_data);
}