#pragma once

#include "zz.h"

struct clock
{
    f64 start_time;
    f64 elapsed_time;
};

void clock_update(struct clock* clock);
void clock_start(struct clock* clock);
void clock_stop(struct clock* clock);