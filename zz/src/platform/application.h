#pragma once

#include "zz/zz.h"

#include "zz/event.h"
#include "zz/input.h"

#include "windows/application.h"

struct platform_application;

struct platform_application_config
{
    struct event* event;
    struct input* input;

    char* name;
    u32 x, y;
    u32 width, height;
};

b8 platform_application_create(struct platform_application* platform_application, struct platform_application_config* config);
void platform_application_destroy(struct platform_application* platform_application);

b8 platform_application_pump_messages(struct platform_application* platform_application);

u64 platform_application_get_time(struct platform_application* platform_application);
void platform_application_sleep(struct platform_application* platform_application, u64 milliseconds);