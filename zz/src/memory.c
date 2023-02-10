#include "zz/memory.h"

#include <string.h>
#include <stdio.h>

#include "zz/log.h"
#include "zz/internal/memory.h"

static struct zz_memory memory;

static const char* memory_tag_strings[ZZ_MEMORY_TAG_MAX] = {
    "UNKNOWN      ",
    "GAME         ",
    "DYNAMIC_ARRAY",
};

b8 zz_memory_initialize(struct zz_memory_config* config)
{
    zz_internal_memory_zero(&memory.statistics, sizeof(memory.statistics));
    ZZ_LOG_INFO("Memory module initialized.");
    return ZZ_TRUE;
}

void zz_memory_deinitialize()
{
    ZZ_LOG_INFO("Memory module deinitialized.");
}

void* zz_memory_allocate(u64 size, enum zz_memory_tag tag)
{
    if (tag == ZZ_MEMORY_TAG_UNKNOWN)
    {
        ZZ_LOG_WARNING("zz_memory_allocate called with tag ZZ_MEMORY_TAG_UNKNOWN");
    }

    // TODO Memory alignment
    void* block = zz_internal_memory_allocate(size, ZZ_FALSE);
    zz_internal_memory_zero(block, size);

    memory.statistics.total_bytes_allocated += size;
    memory.statistics.tagged_bytes_allocated[tag] += size;

    return block;
}

void zz_memory_deallocate(void* block, u64 size, enum zz_memory_tag tag)
{
    if (tag == ZZ_MEMORY_TAG_UNKNOWN)
    {
        ZZ_LOG_WARNING("zz+memory_deallocate called with tag ZZ_MEMORY_TAG_UNKNOWN");
    }

    // TODO Memory alignment
    zz_internal_memory_deallocate(block, ZZ_FALSE);

    memory.statistics.total_bytes_allocated -= size;
    memory.statistics.tagged_bytes_allocated[tag] -= size;
}

void* zz_memory_zero(void* block, u64 size)
{
    return zz_internal_memory_zero(block, size);
}

void* zz_memory_copy(void* destination, const void* source, u64 size)
{
    return zz_internal_memory_copy(destination, source, size);
}

void* zz_memory_set(void* destination, const i32 value, u64 size)
{
    return zz_internal_memory_set(destination, value, size);
}

char* zz_memory_get_usage_string()
{
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "Memory usage:";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < ZZ_MEMORY_TAG_MAX; i += 1)
    {
        char unit[4] = "XiB";
        float amount = 1.0f;
        if (memory.statistics.tagged_bytes_allocated[i] > (float)gib)
        {
            unit[0] = 'G';
            amount = memory.statistics.tagged_bytes_allocated[i] / (float)gib;
        }
        else if (memory.statistics.tagged_bytes_allocated[i] > (float)mib)
        {
            unit[0] = 'M';
            amount = memory.statistics.tagged_bytes_allocated[i] / (float)mib;
        }
        if (memory.statistics.tagged_bytes_allocated[i] > (float)kib)
        {
            unit[0] = 'K';
            amount = memory.statistics.tagged_bytes_allocated[i] / (float)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)memory.statistics.tagged_bytes_allocated[i];
        }

        offset += snprintf(buffer + offset, 8000, "\n  %s: %.2f%s", memory_tag_strings[i], amount, unit);
    }

    char* output = _strdup(buffer);
    return output;
}

void _zz_memory_array_create(void** data, u16* length, u16* capacity, u16 stride)
{
    *data = 0;
    *length = 0;
    *capacity = 0;
}

void _zz_memory_array_create_and_reserve(void** data, u16* length, u16* capacity, u16 stride, u16 n)
{
    _zz_memory_array_create(data, length, capacity, stride);
    _zz_memory_array_reserve(data, length, capacity, stride, n);
}

void _zz_memory_array_destroy(void** data, u16* length, u16* capacity, u16 stride)
{
    zz_memory_deallocate(*data, *capacity * stride, ZZ_MEMORY_TAG_ARRAY);
    *data = 0;
    *length = 0;
    *capacity = 0;
}

void _zz_memory_array_push(void** data, u16* length, u16* capacity, u16 stride, void* input)
{
    _zz_memory_array_expand(data, length, capacity, stride);
    zz_memory_copy((u8*)*data + (*length * stride), input, stride);
    *length += 1;
}

void _zz_memory_array_pop(void** data, u16* length, u16* capacity, u16 stride, void* output)
{
    if (output != ZZ_NULL)
    {
        zz_memory_copy(output, (u8*)*data + ((*length - 1) * stride), stride);
    }
    *length -= 1;
}

void _zz_memory_array_push_at(void** data, u16* length, u16* capacity, u16 stride, u16 index, void* input)
{
    _zz_memory_array_expand_at(data, length, capacity, stride, index);
    zz_memory_copy((u8*)*data + (index * stride), input, stride);
    *length += 1;
}

void _zz_memory_array_pop_at(void** data, u16* length, u16* capacity, u16 stride, u16 index, void* output)
{
    if (output != ZZ_NULL)
    {
        zz_memory_copy(output, (u8*)*data + (index * stride), stride);
    }
    _zz_memory_array_remove_at(data, length, capacity, stride, index);
}

void _zz_memory_array_clear(void** data, u16* length, u16* capacity, u16 stride)
{
    *length = 0;
}

void _zz_memory_array_expand(void** data, u16* length, u16* capacity, u16 stride)
{
    if (*length + 1 > *capacity)
    {
        u16 n = (*capacity == 0) ? 1 : *capacity << 1;
        void* p = zz_memory_allocate(n * stride, ZZ_MEMORY_TAG_ARRAY);
        zz_memory_copy(p, *data, *length * stride);
        zz_memory_deallocate(*data, *capacity * stride, ZZ_MEMORY_TAG_ARRAY);
        *data = p;
        *capacity = n;
    }
}

void _zz_memory_array_contract(void** data, u16* length, u16* capacity, u16 stride)
{
    if (*length == 0)
    {
        zz_memory_deallocate(*data, *capacity * stride, ZZ_MEMORY_TAG_ARRAY);
        *capacity = 0;
    }
    else
    {
        u16 n = *length;
        void* p = zz_memory_allocate(n * stride, ZZ_MEMORY_TAG_ARRAY);
        zz_memory_copy(p, *data, *length * stride);
        zz_memory_deallocate(*data, *capacity * stride, ZZ_MEMORY_TAG_ARRAY);
        *data = p;
        *capacity = n;
    }
}

void _zz_memory_array_reserve(void** data, u16* length, u16* capacity, u16 stride, u16 n)
{
    if (n > *capacity)
    {
        void* p = zz_memory_allocate(n * stride, ZZ_MEMORY_TAG_ARRAY);
        zz_memory_copy(p, *data, *length * stride);
        zz_memory_deallocate(*data, *capacity * stride, ZZ_MEMORY_TAG_ARRAY);
        *data = p;
        *capacity = n;
    }
}

void _zz_memory_array_expand_at(void** data, u16* length, u16* capacity, u16 stride, u16 index)
{
    _zz_memory_array_expand(data, length, capacity, stride);
    zz_memory_copy((u8*)*data + ((index + 1) * stride), (u8*)*data + (index * stride), (*length - index) * stride);
    *length += 1;
}

void _zz_memory_array_remove_at(void** data, u16* length, u16* capacity, u16 stride, u16 index)
{
    if (index != *length - 1)
    {
        zz_memory_copy((u8*)*data + (index * stride), (u8*)*data + ((index + 1) * stride), (*length - index) * stride);
    }
    *length -= 1;
}