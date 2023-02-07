#ifndef ZZ_APPLICATION_H
#define ZZ_APPLICATION_H

#include "zz.h"

#include "memory.h"
#include "zz/internal/application.h"

struct application
{    
    
};

struct application_config
{
    char* name;
    u16 x, y;
    u16 width, height;
};

ZZ_API b8 application_initialize(struct application_config* config);
ZZ_API void application_deinitialize();

ZZ_API void application_get_size(u16* width, u16* height);

ZZ_API u64 application_get_time();
ZZ_API void application_sleep(u64 milliseconds);

#endif