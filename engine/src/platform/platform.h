#pragma once

#include "zz.h"

struct platform
{
    void* state;
};

b8 platform_initialize(struct platform* platform, const char* application_name, i32 x, i32 y, i32 width, i32 height);
void platform_deinitialize(struct platform* platform);

b8 platform_pump_messages(struct platform* platform);

