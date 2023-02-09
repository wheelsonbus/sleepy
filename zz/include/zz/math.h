#ifndef ZZ_MATH_H
#define ZZ_MATH_H

#include "zz.h"

#define ZZ_MATH_PI 3.14159265358979323846f
#define ZZ_MATH_2PI 2.0f * ZZ_MATH_PI
#define ZZ_MATH_PI_OVER_2 0.5f * ZZ_MATH_PI
#define ZZ_MATH_PI_OVER_4 0.25f * ZZ_MATH_PI
#define ZZ_MATH_1_OVER_PI 1.0f / ZZ_MATH_PI
#define ZZ_MATH_1_OVER_2PI 1.0f / ZZ_MATH_2PI
#define ZZ_MATH_SQRT_2 1.41421356237309504880f
#define ZZ_MATH_SQRT_3 1.73205080756887729352f
#define ZZ_MATH_SQRT_1_OVER_2 0.70710678118654752440f
#define ZZ_MATH_SQRT_1_OVER_3 0.57735026918962576450f
#define ZZ_MATH_RADIANS_PER_DEGREE ZZ_MATH_PI / 180.0f
#define ZZ_MATH_DEGREES_PER_RADIAN 180.0f / ZZ_MATH_PI

#define ZZ_MATH_MILLISECONDS_PER_SECOND 1.000f
#define ZZ_MATH_SECONDS_PER_MILLISECOND 0.001f

#define ZZ_MATH_INFINITY 1e38f
#define ZZ_MATH_EPSILON 1.192092896e-07f

typedef union vec2
{
    f32 elements[2];
    struct
    {
        union {f32 x, r, s, u;};
        union {f32 y, g, t, v;};
        
    }; 
}
vec2;

typedef union vec3
{
    f32 elements[3];
    struct
    {
        union {f32 x, r, s, u;};
        union {f32 y, g, t, v;};
        union {f32 z, b, p, w;};
    }; 
}
vec3;

typedef union vec4
{
    f32 elements[4];
    struct
    {
        union {f32 x, r, s;};
        union {f32 y, g, t;};
        union {f32 z, b, p;};
        union {f32 w, a, q;};
    }; 
}
vec4;

typedef vec4 quat;

typedef union mat4
{
    f32 elements[16];

    struct
    {
        struct
        {
            struct
            {
                f32 x, y, z, w;
            } x, y, z, w;
        }; 
    };
} mat4;


ZZ_API f32 f32_sin(f32 x);
ZZ_API f32 f32_cos(f32 x);
ZZ_API f32 f32_tan(f32 x);
ZZ_API f32 f32_acos(f32 x);
ZZ_API f32 f32_sqrt(f32 x);
ZZ_API f32 f32_abs(f32 x);

ZZ_INLINE b8 u64_is_power_of_2(u64 value) {return (value != 0) && ((value & (value - 1)) == 0);}

ZZ_API i32 i32_random();
ZZ_API i32 i32_random_in_range(i32 min, i32 max);

ZZ_INLINE vec2 vec2_fill(f32 x, f32 y) {return (vec2){x, y};}
ZZ_INLINE vec2 vec2_from_vec3(vec3 v) {return (vec2){v.x, v.y};}
ZZ_INLINE vec2 vec2_from_vec4(vec4 v) {return (vec2){v.x, v.y};}
ZZ_INLINE vec2 vec2_zero() {return (vec2){0.0f, 0.0f};}
ZZ_INLINE vec2 vec2_one() {return (vec2){1.0f, 1.0f};}
ZZ_INLINE vec2 vec2_left() {return (vec2){-1.0f, 0.0f};}
ZZ_INLINE vec2 vec2_right() {return (vec2){1.0f, 0.0f};}
ZZ_INLINE vec2 vec2_down() {return (vec2){0.0f, -1.0f};}
ZZ_INLINE vec2 vec2_up() {return (vec2){0.0f, 1.0f};}

ZZ_INLINE b8 vec2_equals(vec2 v0, vec2 v1) {return v0.x == v1.x && v0.y == v1.y;}

ZZ_INLINE vec2 vec2_sum(vec2 v0, vec2 v1) {return (vec2){v0.x + v1.x, v0.y + v1.y};}
ZZ_INLINE vec2 vec2_difference(vec2 v0, vec2 v1) {return (vec2){v0.x - v1.x, v0.y - v1.y};}
ZZ_INLINE vec2 vec2_product(vec2 v0, vec2 v1) {return (vec2){v0.x * v1.x, v0.y * v1.y};}
ZZ_INLINE vec2 vec2_product_scalar(vec3 v, f32 a) {return (vec2){v.x * a, v.y * a};}
ZZ_INLINE vec2 vec2_quotient(vec2 v0, vec2 v1) {return (vec2){v0.x / v1.x, v0.y / v1.y};}

ZZ_INLINE f32 vec2_magnitude_squared(vec2 v) {return (v.x * v.x) + (v.y * v.y);}
ZZ_INLINE f32 vec2_magnitude(vec2 v) {return f32_sqrt(vec2_magnitude_squared(v));}

ZZ_INLINE void vec2_normalize(vec2* v) {const f32 magnitude = vec2_magnitude(*v); v->x /= magnitude; v->y /= magnitude;}
ZZ_INLINE vec2 vec2_normalized(vec2 v) {const f32 magnitude = vec2_magnitude(v); return (vec2){v.x / magnitude, v.y / magnitude};}

ZZ_INLINE f32 vec2_distance_squared(vec2 v0, vec2 v1) {return vec2_magnitude_squared(vec2_difference(v0, v1));}
ZZ_INLINE f32 vec2_distance(vec2 v0, vec2 v1) {return f32_sqrt(vec2_distance_squared(v0, v1));}

ZZ_INLINE f32 vec2_dot_product(vec2 v0, vec2 v1) {return (v0.x * v1.x) + (v0.y * v1.y);}

ZZ_INLINE b8 vec2_compare(vec2 v0, vec2 v1, f32 epsilon) {return (f32_abs(v0.x - v1.x) > epsilon || f32_abs(v0.y - v1.y) > epsilon) ? ZZ_FALSE : ZZ_TRUE;}

ZZ_INLINE vec3 vec3_fill(f32 x, f32 y, f32 z) {return (vec3){x, y, z};}
ZZ_INLINE vec3 vec3_from_vec2(vec2 v, f32 z) {return (vec3){v.x, v.y, z};}
ZZ_INLINE vec3 vec3_from_vec4(vec4 v) {return (vec3){v.x, v.y, v.z};}
ZZ_INLINE vec3 vec3_zero() {return (vec3){0.0f, 0.0f, 0.0f};}
ZZ_INLINE vec3 vec3_one() {return (vec3){1.0f, 1.0f, 1.0f};}
ZZ_INLINE vec3 vec3_left() {return (vec3){-1.0f, 0.0f, 0.0f};}
ZZ_INLINE vec3 vec3_right() {return (vec3){1.0f, 0.0f, 0.0f};}
ZZ_INLINE vec3 vec3_down() {return (vec3){0.0f, -1.0f, 0.0f};}
ZZ_INLINE vec3 vec3_up() {return (vec3){0.0f, 1.0f, 0.0f};}
ZZ_INLINE vec3 vec3_backward() {return (vec3){0.0f, 0.0f, -1.0f};}
ZZ_INLINE vec3 vec3_forward() {return (vec3){0.0f, 0.0f, 1.0f};}

ZZ_INLINE b8 vec3_equals(vec3 v0, vec3 v1) {return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z;}

ZZ_INLINE vec3 vec3_sum(vec3 v0, vec3 v1) {return (vec3){v0.x + v1.x, v0.y + v1.y, v0.z + v1.z};}
ZZ_INLINE vec3 vec3_difference(vec3 v0, vec3 v1) {return (vec3){v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};}
ZZ_INLINE vec3 vec3_product(vec3 v0, vec3 v1) {return (vec3){v0.x * v1.x, v0.y * v1.y, v0.z * v1.z};}
ZZ_INLINE vec3 vec3_product_scalar(vec3 v, f32 a) {return (vec3){v.x * a, v.y * a, v.z * a};}
ZZ_INLINE vec3 vec3_quotient(vec3 v0, vec3 v1) {return (vec3){v0.x / v1.x, v0.y / v1.y, v0.z / v1.z};}

ZZ_INLINE f32 vec3_magnitude_squared(vec3 v) {return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);}
ZZ_INLINE f32 vec3_magnitude(vec3 v) {return f32_sqrt(vec3_magnitude_squared(v));}

ZZ_INLINE void vec3_normalize(vec3* v) {const f32 magnitude = vec3_magnitude(*v); v->x /= magnitude; v->y /= magnitude; v->z /= magnitude;}
ZZ_INLINE vec3 vec3_normalized(vec3 v) {const f32 magnitude = vec3_magnitude(v); return (vec3){v.x / magnitude, v.y / magnitude, v.z / magnitude};}

ZZ_INLINE f32 vec3_distance_squared(vec3 v0, vec3 v1) {return vec3_magnitude_squared(vec3_difference(v0, v1));}
ZZ_INLINE f32 vec3_distance(vec3 v0, vec3 v1) {return f32_sqrt(vec3_distance_squared(v0, v1));}

ZZ_INLINE f32 vec3_dot_product(vec3 v0, vec3 v1) {return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);}
ZZ_INLINE vec3 vec3_cross_product(vec3 v0, vec3 v1) {return (vec3){(v0.y * v1.z) - (v0.z * v1.y), (v0.z * v1.x) - (v0.x * v1.z), (v0.x * v1.y) - (v0.y * v1.x)};}

ZZ_INLINE b8 vec3_compare(vec3 v0, vec3 v1, f32 epsilon) {return (f32_abs(v0.x - v1.x) > epsilon || f32_abs(v0.y - v1.y) > epsilon || f32_abs(v0.z - v0.z) > epsilon) ? ZZ_FALSE : ZZ_TRUE;}

ZZ_INLINE vec4 vec4_fill(f32 x, f32 y, f32 z, f32 w) {return (vec4){x, y, z, w};}
ZZ_INLINE vec4 vec4_from_vec2(vec2 v, f32 z, f32 w) {return (vec4){v.x, v.y, z, w};}
ZZ_INLINE vec4 vec4_from_vec3(vec3 v, f32 w) {return (vec4){v.x, v.y, v.z, w};}
ZZ_INLINE vec4 vec4_zero() {return (vec4){0.0f, 0.0f, 0.0f, 0.0f};}
ZZ_INLINE vec4 vec4_one() {return (vec4){1.0f, 1.0f, 1.0f, 1.0f};}

ZZ_INLINE b8 vec4_equals(vec4 v0, vec4 v1) {return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w;}

ZZ_INLINE vec4 vec4_sum(vec4 v0, vec4 v1) {return (vec4){v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w};}
ZZ_INLINE vec4 vec4_difference(vec4 v0, vec4 v1) {return (vec4){v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w};}
ZZ_INLINE vec4 vec4_product(vec4 v0, vec4 v1) {return (vec4){v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w};}
ZZ_INLINE vec4 vec4_product_scalar(vec4 v, f32 a) {return (vec4){v.x * a, v.y * a, v.z * a, v.w * a};}
ZZ_INLINE vec4 vec4_quotient(vec4 v0, vec4 v1) {return (vec4){v0.x / v1.x, v0.y / v1.y, v0.z / v1.z, v0.w / v1.w};}

ZZ_INLINE f32 vec4_magnitude_squared(vec4 v) {return (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);}
ZZ_INLINE f32 vec4_magnitude(vec4 v) {return f32_sqrt(vec4_magnitude_squared(v));}

ZZ_INLINE void vec4_normalize(vec4* v) {const f32 magnitude = vec4_magnitude(*v); v->x /= magnitude; v->y /= magnitude; v->z /= magnitude; v->w /= magnitude;}
ZZ_INLINE vec4 vec4_normalized(vec4 v) {const f32 magnitude = vec4_magnitude(v); return (vec4){v.x / magnitude, v.y / magnitude, v.z / magnitude, v.w / magnitude};}

ZZ_INLINE f32 vec4_distance_squared(vec4 v0, vec4 v1) {return vec4_magnitude_squared(vec4_difference(v0, v1));}
ZZ_INLINE f32 vec4_distance(vec4 v0, vec4 v1) {return f32_sqrt(vec4_distance_squared(v0, v1));}

ZZ_INLINE f32 vec4_dot_product(vec4 v0, vec4 v1) {return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z) + (v0.w * v1.w);}

ZZ_INLINE b8 vec4_compare(vec4 v0, vec4 v1, f32 epsilon) {return (f32_abs(v0.x - v1.x) > epsilon || f32_abs(v0.y - v1.y) > epsilon || f32_abs(v0.z - v0.z) > epsilon || f32_abs(v0.w - v1.w)) ? ZZ_FALSE : ZZ_TRUE;}

ZZ_INLINE mat4 mat4_identity() {return (mat4){.x = {1.0f, 0.0f, 0.0f, 0.0f}, .y = {0.0f, 1.0f, 0.0f, 0.0f}, .z = {0.0f, 0.0f, 1.0f, 0.0f}, .w = {0.0f, 0.0f, 0.0f, 1.0f}};}

ZZ_INLINE mat4 mat4_orthographic(f32 clip_left, f32 clip_right, f32 clip_top, f32 clip_bottom, f32 clip_near, f32 clip_far) {
    vec3 multiplier = vec3_fill(1.0f / (clip_right - clip_left), 1.0f / (clip_bottom - clip_top), 1.0f / (clip_near - clip_far));
    return (mat4)
    {
        .x = {2.0f * multiplier.x, 0.0f, 0.0f, 0.0f},
        .y = {0.0f, 2.0f * multiplier.y, 0.0f, 0.0f},
        .z = {0.0f, 0.0f, -1.0f * multiplier.z, 0.0f},
        .w = {-(clip_left + clip_right) * multiplier.x, -(clip_top + clip_bottom) * multiplier.y, clip_near * multiplier.z, 1.0f}
    };
}

#endif