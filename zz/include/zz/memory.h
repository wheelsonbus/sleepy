#pragma once

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

ZZ_API b8 memory_create(struct memory* memory, struct memory_config* config);
ZZ_API void memory_destroy(struct memory* memory);

ZZ_API void* memory_allocate(struct memory* memory, u64 size, enum memory_tag tag);
ZZ_API void memory_deallocate(struct memory* memory, void* block, u64 size, enum memory_tag tag);
ZZ_API void* memory_zero(struct memory* memory, void* block, u64 size);
ZZ_API void* memory_copy(struct memory* memory, void* destination, const void* source, u64 size);
ZZ_API void* memory_set(struct memory* memory, void* destination, const i32 value, u64 size);

ZZ_API char* memory_get_usage_string(struct memory* memory);

#define _memory_array_unpack(array) (void**)&(array)->data, &(array)->length, &(array)->capacity, sizeof(*(array)->data)

#define memory_array_t(type) struct {type* data; u16 length, capacity;}

#define memory_array_create(memory, array) _memory_array_create(memory, _memory_array_unpack(array));
#define memory_array_create_and_reserve(memory, array, capacity) _memory_array_create(memory, _memory_array_unpack(array)); _memory_array_reserve(memory, _memory_array_unpack(array), capacity);
#define memory_array_destroy(memory, array) _memory_array_destroy(memory, _memory_array_unpack(array));

#define memory_array_push(memory, array, value) _memory_array_expand(memory, _memory_array_unpack(array)); (array)->data[(array)->length++] = (value)
#define memory_array_pop(memory, array) (array)->data[--(array)->length]
#define memory_array_push_at(memory, array, index, value) _memory_array_expand_at(memory, _memory_array_unpack(array), index); (array)->data[index] = (value)
#define memory_array_pop_at(memory, array, index) (array)->data[index]; _memory_array_remove_at(memory, _memory_array_unpack(array), index)

ZZ_API void _memory_array_create(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_destroy(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride);

ZZ_API void _memory_array_expand(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_contract(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride);
ZZ_API void _memory_array_reserve(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride, u16 n);
ZZ_API void _memory_array_expand_at(struct memory* memory, void** data, u16* length, u16* capacity, u16 stride, u16 index);
ZZ_API void _memory_array_remove_at(struct memory* memory, void ** data, u16* length, u16* capacity, u16 stride, u16 index);