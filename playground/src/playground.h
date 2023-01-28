#pragma once

#include <zz/zz.h>
#include <zz/program.h>
#include <zz/log.h>


b8 playground_initialize();
b8 playground_deinitialize();
b8 playground_update(f64 delta_time);
b8 playground_render(f64 delta_time);

void playground_resize(u32 width, u32 height);
