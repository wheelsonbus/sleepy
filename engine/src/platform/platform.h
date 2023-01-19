#pragma once

#include "definitions.h"

struct platform_state
{
    void* state;
};

ZZ_API b8 platform_initialize(struct platform_state* platform_state, const char* application_name, i32 x, i32 y, i32 width, i32 height);
ZZ_API void platform_deinitialize(struct platform_state* platform_state);

ZZ_API b8 platform_pump_messages(struct platform_state* platform_state);

