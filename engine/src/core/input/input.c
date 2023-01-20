#include "input.h"

#include "core/log/log.h"
#include "core/memory/memory.h"
#include "core/event/event.h"

struct input_state
{
    b8 key_states[ZZ_INPUT_KEY_CODE_MAX];
    b8 mouse_button_states[ZZ_INPUT_MOUSE_BUTTON_CODE_MAX];
    i16 mouse_x, mouse_y;
};

static b8 initialized = FALSE;
static struct input_state input_state = {};
static struct input_state previous_input_state = {};

b8 input_initialize()
{
    memory_zero(&input_state, sizeof(input_state));
    memory_zero(&previous_input_state, sizeof(input_state));
    initialized = TRUE;
    return TRUE;
}

void input_deinitialize()
{
    initialized = FALSE;
}

void input_update(f64 delta_time)
{
    memory_copy(&previous_input_state, &input_state, sizeof(struct input_state));
}

b8 input_get_key_state(enum input_key_code code)
{
    return input_state.key_states[code];
}

b8 input_get_previous_key_state(enum input_key_code code)
{
    return previous_input_state.key_states[code];
}

void input_set_key_state(enum input_key_code code, b8 down)
{
    if (input_state.key_states[code] != down)
    {
        input_state.key_states[code] = down;

        union event_data event_data;
        event_data.u16[0] = (u16)code;
        event_send(down ? ZZ_EVENT_CODE_KEY_PRESSED : ZZ_EVENT_CODE_KEY_RELEASED, 0, event_data);
    }
}

b8 input_get_mouse_button_state(enum input_mouse_button_code code)
{
    return input_state.mouse_button_states[code];
}

b8 input_get_previous_mouse_button_state(enum input_mouse_button_code code)
{
    return previous_input_state.mouse_button_states[code];
}

void input_set_mouse_button_state(enum input_mouse_button_code code, b8 down)
{
    if (input_state.mouse_button_states[code] != down)
    {
        input_state.mouse_button_states[code] = down;

        union event_data event_data;
        event_data.u16[0] = code;
        event_send(down ? ZZ_EVENT_CODE_MOUSE_BUTTON_PRESSED : ZZ_EVENT_CODE_MOUSE_BUTTON_PRESSED, 0, event_data);
    }
}

void input_get_mouse_position(i32* x, i32* y)
{
    *x = input_state.mouse_x;
    *y = input_state.mouse_y;
}

void input_get_previous_mouse_position(i32* x, i32* y)
{
    *x = previous_input_state.mouse_x;
    *y = previous_input_state.mouse_y;
}

void input_set_mouse_position(i16 x, i16 y)
{
    if (input_state.mouse_x != x || input_state.mouse_y != y)
    {
        input_state.mouse_x = x;
        input_state.mouse_y = y;

        union event_data event_data;
        event_data.u16[0] = x;
        event_data.u16[1] = y;
        event_send(ZZ_EVENT_CODE_MOUSE_MOVED, 0, event_data);
    }
}

void input_move_mouse_wheel(i8 delta)
{
        union event_data event_data;
        event_data.u8[0] = delta;
        event_send(ZZ_EVENT_CODE_MOUSE_WHEEL_MOVED, 0, event_data);
}