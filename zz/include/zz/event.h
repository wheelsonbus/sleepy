#pragma once

#include "zz.h"

#include "memory.h"

enum event_code
{
    ZZ_EVENT_CODE_QUIT = 0x01,

    ZZ_EVENT_CODE_KEY_PRESS = 0x02,
    ZZ_EVENT_CODE_KEY_RELEASE = 0x03,

    ZZ_EVENT_CODE_MOUSE_MOVE = 0x04,
    ZZ_EVENT_CODE_MOUSE_BUTTON_PRESS = 0x05,
    ZZ_EVENT_CODE_MOUSE_BUTTON_RELEASE = 0x06,
    ZZ_EVENT_CODE_MOUSE_WHEEL_MOVE = 0x07,

    ZZ_EVENT_CODE_RESIZE = 0x08,

    ZZ_EVENT_CODE_MAX = 0x100,
};

struct event_data_null
{
    i64 i64[2];
};

union event_data
{
    struct event_data_null null;

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

typedef b8 (*event_callback_t)(void* sender, void* receiver, union event_data data);

struct event_receiver_registration
{
    void* receiver;
    event_callback_t callback;
};

struct event_code_registry
{
    memory_array_t(struct event_receiver_registration) registrations;
};

struct event
{    
    struct event_code_registry registries[ZZ_EVENT_CODE_MAX];
};

struct event_config
{

};

ZZ_API b8 event_initialize(struct event_config* config);
ZZ_API void event_deinitialize();

ZZ_API b8 event_register_receiver(u16 code, void* receiver, event_callback_t callback);
ZZ_API b8 event_unregister_receiver(u16 code, void* receiver, event_callback_t callback);

ZZ_API b8 event_send(u16 code, void* sender, union event_data data);
ZZ_API b8 event_send_null(u16 code, void* sender);