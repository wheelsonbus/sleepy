#include "zz/time.h"

#include "platform/time.h"

f64 time_get()
{
    return platform_time_get();
}

void time_sleep(u64 milliseconds)
{
    platform_time_sleep(milliseconds);
}