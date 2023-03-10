#ifndef ZZ_INPUT_H
#define ZZ_INPUT_H

#include "zz.h"

#include "memory.h"
#include "event.h"

enum zz_input_mouse_button_code
{
    ZZ_INPUT_MOUSE_BUTTON_CODE_LEFT,
    ZZ_INPUT_MOUSE_BUTTON_CODE_MIDDLE,
    ZZ_INPUT_MOUSE_BUTTON_CODE_RIGHT,

    ZZ_INPUT_MOUSE_BUTTON_CODE_MAX
};

enum zz_input_key_code
{
    ZZ_INPUT_KEY_CODE_BACKSPACE = 0x08,
    ZZ_INPUT_KEY_CODE_ENTER = 0x0D,
    ZZ_INPUT_KEY_CODE_TAB = 0x09,
    ZZ_INPUT_KEY_CODE_SHIFT = 0x10,
    ZZ_INPUT_KEY_CODE_CONTROL = 0x11,

    ZZ_INPUT_KEY_CODE_PAUSE = 0x13,
    ZZ_INPUT_KEY_CODE_CAPITAL = 0x14,

    ZZ_INPUT_KEY_CODE_ESCAPE = 0x1B,

    ZZ_INPUT_KEY_CODE_SPACE = 0x20,

    ZZ_INPUT_KEY_CODE_LEFT = 0x25,
    ZZ_INPUT_KEY_CODE_UP = 0x26,
    ZZ_INPUT_KEY_CODE_RIGHT = 0x27,
    ZZ_INPUT_KEY_CODE_DOWN = 0x28,

    ZZ_INPUT_KEY_CODE_A = 0x41,
    ZZ_INPUT_KEY_CODE_B = 0x42,
    ZZ_INPUT_KEY_CODE_C = 0x43,
    ZZ_INPUT_KEY_CODE_D = 0x44,
    ZZ_INPUT_KEY_CODE_E = 0x45,
    ZZ_INPUT_KEY_CODE_F = 0x46,
    ZZ_INPUT_KEY_CODE_G = 0x47,
    ZZ_INPUT_KEY_CODE_H = 0x48,
    ZZ_INPUT_KEY_CODE_I = 0x49,
    ZZ_INPUT_KEY_CODE_J = 0x4A,
    ZZ_INPUT_KEY_CODE_K = 0x4B,
    ZZ_INPUT_KEY_CODE_L = 0x4C,
    ZZ_INPUT_KEY_CODE_M = 0x4D,
    ZZ_INPUT_KEY_CODE_N = 0x4E,
    ZZ_INPUT_KEY_CODE_O = 0x4F,
    ZZ_INPUT_KEY_CODE_P = 0x50,
    ZZ_INPUT_KEY_CODE_Q = 0x51,
    ZZ_INPUT_KEY_CODE_R = 0x52,
    ZZ_INPUT_KEY_CODE_S = 0x53,
    ZZ_INPUT_KEY_CODE_T = 0x54,
    ZZ_INPUT_KEY_CODE_U = 0x55,
    ZZ_INPUT_KEY_CODE_V = 0x56,
    ZZ_INPUT_KEY_CODE_W = 0x57,
    ZZ_INPUT_KEY_CODE_X = 0x58,
    ZZ_INPUT_KEY_CODE_Y = 0x59,
    ZZ_INPUT_KEY_CODE_Z = 0x5A,

    ZZ_INPUT_KEY_CODE_SLEEP = 0x5F,

    ZZ_INPUT_KEY_CODE_NUMPAD_0 = 0x60,
    ZZ_INPUT_KEY_CODE_NUMPAD_1 = 0x61,
    ZZ_INPUT_KEY_CODE_NUMPAD_2 = 0x62,
    ZZ_INPUT_KEY_CODE_NUMPAD_3 = 0x63,
    ZZ_INPUT_KEY_CODE_NUMPAD_4 = 0x64,
    ZZ_INPUT_KEY_CODE_NUMPAD_5 = 0x65,
    ZZ_INPUT_KEY_CODE_NUMPAD_6 = 0x66,
    ZZ_INPUT_KEY_CODE_NUMPAD_7 = 0x67,
    ZZ_INPUT_KEY_CODE_NUMPAD_8 = 0x68,
    ZZ_INPUT_KEY_CODE_NUMPAD_9 = 0x69,
    ZZ_INPUT_KEY_CODE_NUMPAD_MULTIPLY = 0x6A,
    ZZ_INPUT_KEY_CODE_NUMPAD_ADD = 0x6B,
    ZZ_INPUT_KEY_CODE_NUMPAD_SEPARATOR = 0x6C,
    ZZ_INPUT_KEY_CODE_NUMPAD_SUBTRACT = 0x6D,
    ZZ_INPUT_KEY_CODE_NUMPAD_DECIMAL = 0x6E,
    ZZ_INPUT_KEY_CODE_NUMPAD_DIVIDE = 0x6F,
    ZZ_INPUT_KEY_CODE_F1 = 0x70,
    ZZ_INPUT_KEY_CODE_F2 = 0x71,
    ZZ_INPUT_KEY_CODE_F3 = 0x72,
    ZZ_INPUT_KEY_CODE_F4 = 0x73,
    ZZ_INPUT_KEY_CODE_F5 = 0x74,
    ZZ_INPUT_KEY_CODE_F6 = 0x75,
    ZZ_INPUT_KEY_CODE_F7 = 0x76,
    ZZ_INPUT_KEY_CODE_F8 = 0x77,
    ZZ_INPUT_KEY_CODE_F9 = 0x78,
    ZZ_INPUT_KEY_CODE_F10 = 0x79,
    ZZ_INPUT_KEY_CODE_F11 = 0x7A,
    ZZ_INPUT_KEY_CODE_F12 = 0x7B,
    ZZ_INPUT_KEY_CODE_F13 = 0x7C,
    ZZ_INPUT_KEY_CODE_F14 = 0x7D,
    ZZ_INPUT_KEY_CODE_F15 = 0x7E,
    ZZ_INPUT_KEY_CODE_F16 = 0x7F,
    ZZ_INPUT_KEY_CODE_F17 = 0x80,
    ZZ_INPUT_KEY_CODE_F18 = 0x81,
    ZZ_INPUT_KEY_CODE_F19 = 0x82,
    ZZ_INPUT_KEY_CODE_F20 = 0x83,
    ZZ_INPUT_KEY_CODE_F21 = 0x84,
    ZZ_INPUT_KEY_CODE_F22 = 0x85,
    ZZ_INPUT_KEY_CODE_F23 = 0x86,
    ZZ_INPUT_KEY_CODE_F24 = 0x87,

    ZZ_INPUT_KEY_CODE_NUM_LOCK = 0x90,
    ZZ_INPUT_KEY_CODE_SCROLL_LOCK = 0x91,

    ZZ_INPUT_KEY_CODE_NUMPAD_EQUALS = 0x92,

    ZZ_INPUT_KEY_CODE_LEFT_SHIFT = 0xA0,
    ZZ_INPUT_KEY_CODE_RIGHT_SHIFT = 0xA1,
    ZZ_INPUT_KEY_CODE_LEFT_CONTROL = 0xA2,
    ZZ_INPUT_KEY_CODE_RIGHT_CONTROL = 0xA3,

    ZZ_INPUT_KEY_CODE_SEMICOLON = 0xBA,
    ZZ_INPUT_KEY_CODE_PLUS = 0xBB,
    ZZ_INPUT_KEY_CODE_COMMA = 0xBC,
    ZZ_INPUT_KEY_CODE_MINUS = 0xBD,
    ZZ_INPUT_KEY_CODE_PERIOD = 0xBE,
    ZZ_INPUT_KEY_CODE_SLASH = 0xBF,
    ZZ_INPUT_KEY_CODE_GRAVE = 0xC0,

    ZZ_INPUT_KEY_CODE_MAX
};

struct zz_input_state
{
    b8 key_states[ZZ_INPUT_KEY_CODE_MAX];
    b8 mouse_button_states[ZZ_INPUT_MOUSE_BUTTON_CODE_MAX];
    i16 mouse_x, mouse_y;
};

struct zz_input
{
    struct zz_input_state state;
    struct zz_input_state previous_state;
};

struct zz_input_config
{
    
};

ZZ_API b8 zz_input_initialize(struct zz_input_config* config);
ZZ_API void zz_input_deinitialize();

void zz_input_update();

ZZ_API b8 zz_input_get_key_state(enum zz_input_key_code code);
ZZ_API b8 zz_input_get_previous_key_state(enum zz_input_key_code code);
void zz_input_set_key_state(enum zz_input_key_code code, b8 down);

ZZ_API b8 zz_input_get_mouse_button_state(enum zz_input_mouse_button_code code);
ZZ_API b8 zz_input_get_mouse_button_state(enum zz_input_mouse_button_code code);
void zz_input_set_mouse_button_state(enum zz_input_mouse_button_code code, b8 down);

ZZ_API void zz_input_get_mouse_position(i32* x, i32* y);
ZZ_API void zz_input_get_previous_mouse_position(i32* x, i32* y);
void zz_input_set_mouse_position(i16 x, i16 y);

void zz_input_move_mouse_wheel(i8 delta);

#endif