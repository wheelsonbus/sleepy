#pragma once

#include <zz/zz.h>
#include <zz/log.h>
#include <zz/program.h>

#include "box.h"

b8 playground_on_initialize(struct program* program);
b8 playground_on_deinitialize(struct program* program);
b8 playground_on_tick(struct program* program, f64 delta_time);
b8 playground_on_frame(struct program* program, f64 delta_time);
