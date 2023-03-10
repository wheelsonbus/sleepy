#ifdef ZZ_WINDOWS

#include "zz/internal/memory.h"

#include <stdlib.h>
#include <string.h>

void* zz_internal_memory_allocate(u64 size, b8 aligned)
{
    return malloc(size);
}

void zz_internal_memory_deallocate(void* block, b8 aligned)
{
    free(block);
}

void* zz_internal_memory_zero(void* block, u64 size)
{
    return memset(block, 0, size);
}

void* zz_internal_memory_copy(void* destination, const void* source, u64 size)
{
    return memcpy(destination, source, size);
}

void* zz_internal_memory_set(void* destination, const i32 value, u64 size)
{
    return memset(destination, value, size);
}

#endif