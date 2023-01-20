#pragma once

#include "zz.h"

#include "platform/application/application.h"

b8 backend_initialize(struct platform_application* platform_application, const char* application_name);
void backend_deinitialize();

void backend_resize(u16 width, u16 height);

b8 backend_begin_frame(f32 delta_time);
b8 backend_end_frame(f32 delta_time);
