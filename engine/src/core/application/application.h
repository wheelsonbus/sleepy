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
void application_deinitialize(struct program* program);

ZZ_API b8 application_run();