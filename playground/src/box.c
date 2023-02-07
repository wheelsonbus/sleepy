#include "box.h"

void box_draw(struct box* box)
{
    render_draw_sprite(&box->sprite, box->position);
}