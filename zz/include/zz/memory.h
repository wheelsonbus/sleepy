#ifndef ZZ_MEMORY_H
#define ZZ_MEMORY_H

#include "zz.h"

enum memory_tag
{
    ZZ_MEMORY_TAG_UNKNOWN,
    ZZ_MEMORY_TAG_PROGRAM,
    ZZ_MEMORY_TAG_RENDER,
    ZZ_MEMORY_TAG_STRING,
    ZZ_MEMORY_TAG_ARRAY,

    ZZ_MEMORY_TAG_MAX,
};

struct memory_statistics
{
    u64 total_bytes_allocated;
    u64 tagged_bytes_allocated[ZZ_MEMORY_TAG_MAX];
};

struct memory
{
    struct memory_statistics statistics;
};

struct memory_config
{

}; 

ZZ_API b8 memory_initialize(struct memory_config* config);
ZZ_API void memory_deinitialize();

ZZ_API void* memory_allocate(u64 size, enum memory_tag tag);
ZZ_API void memory_deallocate(void* block, u64 size, enum memory_tag tag);
ZZ_API void* memory_zero(void* block, u64 size);
ZZ_API void* memory_copy(void* destination, const void* source, u64 size);
ZZ_API void* memory_set(void* destination, const i32 value, u64 size);

ZZ_API char* memory_get_usage_string();

#define _memory_array_unpack(array) (void**)&(array)->data, &(array)->length, &(array)->capacity, sizeof(*(array)->data)

#define memory_array_t(type) struct {type* data; u16 length, capacity;}

#define memory_array_create(array) _memory_array_create(_memory_array_unpack(array))
#define memory_array_create_and_reserve(array, capacity) _memory_array_create(_memory_array_unpack(array)); _memory_array_reserve(_memory_array_unpack(array), capacity)
#define memory_array_destroy(array) _memory_array_destroy(_memory_array_unpack(array))

#define memory_array_push(array, value) _memory_array_expand(_memory_array_unpack(array)); (array)->data[(array)->length++] = (value)
#define memory_array_pop(array) (array)->data[--(array)->length]
#define memory_array_push_at(array, index, value) _memory_array_expand_at(_memory_array_unpack(array), index); (array)->data[index] = (value)
#define memory_array_pop_at(array, index) (array)->data[index]; _memory_array_remove_at(_memory_array_unpack(array), index)
#define memory_array_clear(array) _memory_array_clear(_memory_array_unpack(array))

ZZ_API void _memory_array_create(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_destroy(void** data, u16* length, u16* capacity, u16 stride);

ZZ_API void _memory_array_expand(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_contract(void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_reserve(void** data, u16* length, u16* capacity, u16 stride, u16 n);
ZZ_API void _memory_array_expand_at(void** data, u16* length, u16* capacity, u16 stride, u16 index);
ZZ_API void _memory_array_remove_at(void** data, u16* length, u16* capacity, u16 stride, u16 index);
ZZ_API void _memory_array_clear(void** data, u16* length, u16* capacity, u16 stride);

#endif