#ifndef PLAYGROUND_BOX_H
#define PLAYGROUND_BOX_H

#include <playground/playground.h>

#include <zz/zz.h>
#include <zz/math.h>
#include <zz/client.h>
#include <zz/sprite.h>

struct box
{
    vec3 position;
    struct zz_sprite sprite;
};
typedef struct {struct box* data; u16 length, capacity;} zz_memory_array_box_t;

PLAYGROUND_API void box_draw(struct box* box);

#endif