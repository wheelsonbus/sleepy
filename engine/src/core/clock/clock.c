#include "clock.h"

#include "core/time/time.h"

void clock_update(struct clock* clock)
{
    if (clock->start_time != 0)
    {
        clock->elapsed_time = time_get() - clock->start_time;
    }
}

void clock_start(struct clock* clock)
{
    clock->start_time = time_get();
    clock->elapsed_time = 0;
}

void clock_stop(struct clock* clock)
{
    clock->start_time = 0;
}