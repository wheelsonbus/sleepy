#pragma once

#include "zz/zz.h"

void* internal_memory_allocate(u64 size, b8 aligned);
void internal_memory_deallocate(void* block, b8 aligned);
void* internal_memory_zero(void* block, u64 size);
void* internal_memory_copy(void* destination, const void* source, u64 size);
void* internal_memory_set(void* destination, const i32 value, u64 size);