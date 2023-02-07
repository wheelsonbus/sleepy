#ifndef ZZ_INTERNAL_APPLICATION_H
#define ZZ_INTERNAL_APPLICATION_H

#include "zz/zz.h"

#include "zz/event.h"
#include "zz/input.h"

struct zz_internal_application_config
{
    char* name;
    u32 x, y;
    u32 width, height;
};

b8 zz_internal_application_initialize(struct zz_internal_application_config* config);
void zz_internal_application_deinitialize();

b8 zz_internal_application_pump_messages();

void zz_internal_application_get_size(u16* width, u16* height);

void zz_internal_application_initialize_time(); // FIXME Abstract time away from the application
u64 zz_internal_application_get_time();
void zz_internal_application_sleep(u64 milliseconds);

#endif