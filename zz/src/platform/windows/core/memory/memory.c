#if defined(ZZ_PLATFORM_WINDOWS)

#include "platform/memory.h"

#include <stdlib.h>
#include <string.h>

void* platform_memory_allocate(u64 size, b8 aligned)
{
    return malloc(size);
}

void platform_memory_deallocate(void* block, b8 aligned)
{
    free(block);
}

void* platform_memory_zero(void* block, u64 size)
{
    return memset(block, 0, size);
}

void* platform_memory_copy(void* destination, const void* source, u64 size)
{
    return memcpy(destination, source, size);
}

void* platform_memory_set(void* destination, const i32 value, u64 size)
{
    return memset(destination, value, size);
}

#endif