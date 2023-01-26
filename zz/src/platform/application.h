#pragma once

#include "zz/zz.h"

struct platform_application
{
    void* state;
};

b8 platform_application_initialize(struct platform_application* application, const char* name, i32 x, i32 y, i32 width, i32 height);
void platform_application_deinitialize(struct platform_application* application);

b8 platform_application_pump_messages(struct platform_application* application);