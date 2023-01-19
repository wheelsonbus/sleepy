#pragma once

#include "definitions.h"
#include "platform/platform.h"

f64 time_get(struct platform_state* platform_state);
void time_sleep(u64 milliseconds);