#pragma once

#include "zz.h"

enum event_code
{
    ZZ_EVENT_CODE_QUIT = 0x01,

    ZZ_EVENT_CODE_KEY_PRESSED = 0x02,
    ZZ_EVENT_CODE_KEY_RELEASED = 0x03,

    ZZ_EVENT_CODE_MOUSE_MOVED = 0x04,
    ZZ_EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x05,
    ZZ_EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x06,
    ZZ_EVENT_CODE_MOUSE_WHEEL_MOVED = 0x07,

    ZZ_EVENT_CODE_RESIZED = 0x08,

    ZZ_EVENT_CODE_MAX = 0xFF,
};

union event_data
{
    i64 i64[2];
    u64 u64[2];
    f64 f64[2];
    
    i32 i32[4];
    u32 u32[4];
    f32 f32[4];
    
    i16 i16[8];
    u16 u16[8];

    i8 i8[16];
    u8 u8[16];

    char c[16];
};

typedef b8 (*event_callback_t)(u16 code, void* sender, void* receiver, union event_data data);

ZZ_API b8 event_initialize();
ZZ_API void event_deinitialize();

ZZ_API b8 event_register_receiver(u16 code, void* receiver, event_callback_t callback);
ZZ_API b8 event_unregister_receiver(u16 code, void* receiver, event_callback_t callback);

ZZ_API b8 event_send(u16 code, void* sender, union event_data data);