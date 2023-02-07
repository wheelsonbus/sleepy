#include "playground/box.h"

#include <zz/render.h>

void box_draw(struct box* box)
{
    render_draw_sprite(&box->sprite, box->position);
}