#include "box.h"

void box_draw(struct box* box, struct program* program)
{
    program_draw_sprite(program, &box->sprite, box->position);
}