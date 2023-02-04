#pragma once

#include <zz/zz.h>
#include <zz/math.h>

#include <zz/sprite.h>

struct box
{
    vec3 position;
    struct sprite sprite;
};
typedef struct {struct box* data; u16 length, capacity;} memory_array_box_t;