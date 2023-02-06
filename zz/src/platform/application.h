#pragma once

#include "zz/zz.h"

#include "zz/event.h"
#include "zz/input.h"

struct platform_application_config
{
    char* name;
    u32 x, y;
    u32 width, height;
};

b8 platform_application_initialize(struct platform_application_config* config);
void platform_application_deinitialize();

b8 platform_application_pump_messages();

void platform_application_get_size(u16* width, u16* height);

u64 platform_application_get_time();
void platform_application_sleep(u64 milliseconds);