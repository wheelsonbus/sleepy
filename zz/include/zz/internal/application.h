#ifndef ZZ_INTERNAL_APPLICATION_H
#define ZZ_INTERNAL_APPLICATION_H

#include "zz/zz.h"

#include "zz/event.h"
#include "zz/input.h"

struct internal_application_config
{
    char* name;
    u32 x, y;
    u32 width, height;
};

b8 internal_application_initialize(struct internal_application_config* config);
void internal_application_deinitialize();

b8 internal_application_pump_messages();

void internal_application_get_size(u16* width, u16* height);

void internal_application_initialize_time(); // FIXME Abstract time away from the application
u64 internal_application_get_time();
void internal_application_sleep(u64 milliseconds);

#endif