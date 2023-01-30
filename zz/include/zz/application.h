#pragma once

#include "zz.h"

#include "memory.h"
#include "platform/application.h"

struct application
{
    struct memory* memory;
    struct event* event;
    struct input* input;
    
    struct platform_application platform_application;
};

struct application_config
{
    struct memory* memory;
    struct event* event;
    struct input* input;

    char* name;
    u16 x, y;
    u16 width, height;
};

ZZ_API b8 application_create(struct application* application, struct application_config* config);
ZZ_API void application_destroy(struct application* application);

ZZ_API u64 application_get_time(struct application* application);
ZZ_API void application_sleep(struct application* application, u64 milliseconds);