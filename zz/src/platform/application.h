#pragma once

#include "zz/zz.h"

#include "zz/event.h"
#include "zz/input.h"

struct platform_application
{
    void* state;
};

b8 platform_application_initialize(struct platform_application* application, const char* name, u32 x, u32 y, u32 width, u32 height, struct event* event, struct input* input);
void platform_application_deinitialize(struct platform_application* application);

b8 platform_application_pump_messages(struct platform_application* application);

u64 platform_application_get_time(struct platform_application* application);
void platform_application_sleep(struct platform_application* application, u64 milliseconds);