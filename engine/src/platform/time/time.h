#pragma once

#include "zz.h"
#include "platform/platform.h"

f64 platform_time_get(struct platform* platform);
void platform_time_sleep(u64 milliseconds);