#pragma once

#include "zz.h"

struct program;

struct application_config
{
    i16 x, y;
    i16 width, height;
    char* name;
};

ZZ_API b8 application_initialize(struct program* program);
ZZ_API void application_deinitialize();

ZZ_API b8 application_run();

void application_get_size(u32* width, u32* height);