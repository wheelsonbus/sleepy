#pragma once

#include "zz.h"

enum memory_tag
{
    ZZ_MEMORY_TAG_UNKNOWN,
    ZZ_MEMORY_TAG_PROGRAM,
    ZZ_MEMORY_TAG_RENDER,
    ZZ_MEMORY_TAG_STRING,
    ZZ_MEMORY_TAG_DYNAMIC_ARRAY,

    ZZ_MEMORY_TAG_MAX,
};

ZZ_API b8 memory_initialize();
ZZ_API void memory_deinitialize();

ZZ_API void* memory_allocate(u64 size, enum memory_tag tag);
ZZ_API void memory_deallocate(void* block, u64 size, enum memory_tag tag);
ZZ_API void* memory_zero(void* block, u64 size);
ZZ_API void* memory_copy(void* destination, const void* source, u64 size);
ZZ_API void* memory_set(void* destination, const i32 value, u64 size);

ZZ_API char* memory_get_usage_string();