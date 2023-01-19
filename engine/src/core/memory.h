#pragma once

#include "definitions.h"

void* memory_allocate(u64 size, b8 aligned);
void memory_deallocate(void* block, u64 size);
void* memory_zero(void* block, u64 size);
void* memory_copy(void* destination, const void* source, u64 size);
void* memory_set(void* destination, const i32 value, u64 size);