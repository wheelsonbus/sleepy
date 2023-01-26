#pragma once

#include "zz.h"

#include "application.h"
#include "platform/application.h"

struct render_packet
{
    f32 delta_time;
};

b8 render_initialize(const char* application_name, struct platform_application* platform_application);
void render_deinitialize();

void render_resize(u16 width, u16 height);

b8 render_draw_frame(struct render_packet* packet);

void render_get_size(u32* width, u32* height);