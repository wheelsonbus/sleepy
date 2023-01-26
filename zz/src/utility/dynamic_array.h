#pragma once

#include "zz/zz.h"

enum dynamic_array_field
{
    ZZ_DYNAMIC_ARRAY_FIELD_CAPACITY,
    ZZ_DYNAMIC_ARRAY_FIELD_LENGTH,
    ZZ_DYNAMIC_ARRAY_FIELD_STRIDE,
    ZZ_DYNAMIC_ARRAY_FIELD_MAX,
};

ZZ_API void* _dynamic_array_create(u64 capacity, u64 stride);
ZZ_API void _dynamic_array_destroy(void* dynamic_array);

ZZ_API u64 _dynamic_array_get_field(void* dynamic_array, u64 field);
ZZ_API void _dynamic_array_set_field(void* dynamic_array, u64 field, u64 value);

ZZ_API void* _dynamic_array_resize(void* dynamic_array);

ZZ_API void* _dynamic_array_push(void* dynamic_array, const void* source);
ZZ_API void* _dynamic_array_pop(void* dynamic_array, void* destination);

ZZ_API void* _dynamic_array_insert_at(void* dynamic_array, u64 index, const void* source);
ZZ_API void* _dynamic_array_pop_at(void* dynamic_array, u64 index, void* destination);

#define ZZ_DYNAMIC_ARRAY_DEFAULT_CAPACITY 1
#define ZZ_DYNAMIC_ARRAY_RESIZE_FACTOR 2

#define dynamic_array_create(type) _dynamic_array_create(ZZ_DYNAMIC_ARRAY_DEFAULT_CAPACITY, sizeof(type))
#define dynamic_array_create_and_reserve(type, capacity) _dynamic_array_create(capacity, sizeof(type))
#define dynamic_array_destroy(dynamic_array) _dynamic_array_destroy(dynamic_array)

#define dynamic_array_push(dynamic_array, value) {typeof(value) temp = value; dynamic_array = _dynamic_array_push(dynamic_array, &temp);}
#define dynamic_array_pop(dynamic_array, destination) _dynamic_array_pop(dynamic_array, destination)

#define dynamic_array_insert_at(dynamic_array, index, value) {typeof(value) temp = value; dynamic_array = _dynamic_array_insert_at(dynamic_array, index, &temp);}
#define dynamic_array_pop_at(dynamic_array, index, destination) _dynamic_array_pop_at(dynamic_array, index, destination)

#define dynamic_array_clear(dynamic_array) _dynamic_array_set_field(array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, 0)

#define dynamic_array_get_capacity(dynamic_array) _dynamic_array_get_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_CAPACITY)
#define dynamic_array_get_length(dynamic_array) _dynamic_array_get_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH)
#define dynamic_array_get_stride(dynamic_array) _dynamic_array_get_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_STRIDE)
