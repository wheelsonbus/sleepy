#include "zz/math.h"

#include <math.h>
#include <stdlib.h>

static b8 math_random_seeded = FALSE;

f32 f32_sin(f32 x) {return sinf(x);}
f32 f32_cos(f32 x) {return cosf(x);}
f32 f32_tan(f32 x) {return tanf(x);}
f32 f32_acos(f32 x) {return acos(x);}
f32 f32_sqrt(f32 x) {return sqrt(x);}
f32 f32_abs(f32 x) {return fabsf(x);}

i32 i32_random() {if (!math_random_seeded) {srand(0);} return rand();}
i32 i32_random_in_range(i32 min, i32 max) {return (i32_random() % (max - min)) + min;}