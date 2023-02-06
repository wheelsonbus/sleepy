#include "box.h"

void box_draw(struct box* box)
{
    program_draw_sprite(&box->sprite, box->position);
}