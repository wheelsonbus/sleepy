#pragma once

#include <zz/zz.h>
#include <zz/log.h>
#include <zz/program.h>

b8 playground_on_initialize();
b8 playground_on_deinitialize();
b8 playground_on_tick(u64 delta_time);
b8 playground_on_frame(u64 delta_time);
