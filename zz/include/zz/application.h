#ifndef ZZ_APPLICATION_H
#define ZZ_APPLICATION_H

#include "zz.h"

#include "memory.h"
#include "zz/internal/application.h"

struct zz_applicatoin
{

};

struct zz_application_config
{
    char* name;
    u16 x, y;
    u16 width, height;
};

ZZ_API b8 zz_application_initialize(struct zz_application_config* config);
ZZ_API void zz_application_deinitialize();

ZZ_API void zz_application_get_size(u16* width, u16* height);

ZZ_API u64 zz_application_get_time();
ZZ_API void zz_application_sleep(u64 milliseconds);

#endif