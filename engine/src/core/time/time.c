#include "time.h"

#include "platform/time/time.h"

f64 time_get(struct platform* platform)
{
    return platform_time_get(platform);
}

void time_sleep(u64 milliseconds)
{
    return platform_time_sleep(milliseconds);
}