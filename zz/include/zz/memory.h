#ifndef ZZ_MEMORY_H
#define ZZ_MEMORY_H

#include "zz.h"

enum zz_memory_tag
{
    ZZ_MEMORY_TAG_UNKNOWN,
    ZZ_MEMORY_TAG_CLIENT,
    ZZ_MEMORY_TAG_RENDER,
    ZZ_MEMORY_TAG_STRING,
    ZZ_MEMORY_TAG_ARRAY,
    ZZ_MEMORY_TAG_ASSET,

    ZZ_MEMORY_TAG_MAX,
};

struct zz_memory_statistics
{
    u64 total_bytes_allocated;
    u64 tagged_bytes_allocated[ZZ_MEMORY_TAG_MAX];
};

struct zz_memory
{
    struct zz_memory_statistics statistics;
};

struct zz_memory_config
{

}; 

ZZ_API b8 zz_memory_initialize(struct zz_memory_config* config);
ZZ_API void zz_memory_deinitialize();

ZZ_API void* zz_memory_allocate(u64 size, enum zz_memory_tag tag);
ZZ_API void zz_memory_deallocate(void* block, u64 size, enum zz_memory_tag tag);
ZZ_API void* zz_memory_zero(void* block, u64 size);
ZZ_API void* zz_memory_copy(void* destination, const void* source, u64 size);
ZZ_API void* zz_memory_set(void* destination, const i32 value, u64 size);

ZZ_API char* zz_memory_get_usage_string();

#define _zz_memory_array_unpack(array) (void**)&(array)->data, &(array)->length, &(array)->capacity, sizeof(*(array)->data)

#define zz_memory_array_t(type) struct {type* data; u16 length, capacity;}

#define zz_memory_array_create(array) _zz_memory_array_create(_zz_memory_array_unpack(array))
#define zz_memory_array_create_and_reserve(array, capacity) _zz_memory_array_create_and_reserve(_zz_memory_array_unpack(array), capacity)
#define zz_memory_array_destroy(array) _zz_memory_array_destroy(_zz_memory_array_unpack(array))

#define zz_memory_array_push(array, input) _zz_memory_array_push(_zz_memory_array_unpack(array), input)
#define zz_memory_array_pop(array, output) _zz_memory_array_pop(_zz_memory_array_unpack(array), output)
#define zz_memory_array_push_at(array, index, input) _zz_memory_array_push_at(_zz_memory_array_unpack(array), index, input)
#define zz_memory_array_pop_at(array, index, output) _zz_memory_array_pop_at(_zz_memory_array_unpack(array), index, output)
#define zz_memory_array_clear(array) _zz_memory_array_clear(_zz_memory_array_unpack(array))

#define zz_memory_array_contract(array) _zz_memory_array_contract(_zz_memory_array_unpack(array));

ZZ_API void _zz_memory_array_create(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _zz_memory_array_create_and_reserve(void** data, u16* length, u16* capacity, u16 stride, u16 n);
ZZ_API void _zz_memory_array_destroy(void** data, u16* length, u16* capacity, u16 stride);

ZZ_API void _zz_memory_array_push(void** data, u16* length, u16* capacity, u16 stride, void* input);
ZZ_API void _zz_memory_array_pop(void** data, u16* length, u16* capacity, u16 stride, void* output);
ZZ_API void _zz_memory_array_push_at(void** data, u16* length, u16* capacity, u16 stride, u16 index, void* input);
ZZ_API void _zz_memory_array_pop_at(void** data, u16* length, u16* capacity, u16 stride, u16 index, void* output);
ZZ_API void _zz_memory_array_clear(void** data, u16* length, u16* capacity, u16 stride);

ZZ_API void _zz_memory_array_expand(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _zz_memory_array_contract(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _zz_memory_array_reserve(void** data, u16* length, u16* capacity, u16 stride, u16 n);
ZZ_API void _zz_memory_array_expand_at(void** data, u16* length, u16* capacity, u16 stride, u16 index);
ZZ_API void _zz_memory_array_remove_at(void** data, u16* length, u16* capacity, u16 stride, u16 index);

#endif