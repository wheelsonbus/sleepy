#pragma once

#include "zz.h"
#include "platform/platform.h"
#include "core/logger/logger.h"

struct game;

struct application_config
{
    i16 x, y;
    i16 width, height;
    char* name;
};

ZZ_API b8 application_initialize(struct game* game);

ZZ_API b8 application_run();