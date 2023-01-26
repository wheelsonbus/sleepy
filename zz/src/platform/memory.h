#pragma once

#include "zz/zz.h"

void* platform_memory_allocate(u64 size, b8 aligned);
void platform_memory_deallocate(void* block, b8 aligned);
void* platform_memory_zero(void* block, u64 size);
void* platform_memory_copy(void* destination, const void* source, u64 size);
void* platform_memory_set(void* destination, const i32 value, u64 size);