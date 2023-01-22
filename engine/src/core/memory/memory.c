#include "memory.h"

#include <string.h>
#include <stdio.h>

#include "core/log/log.h"
#include "platform/core/memory/memory.h"

struct memory_statistics
{
    u64 total_bytes_allocated;
    u64 tagged_bytes_allocated[ZZ_MEMORY_TAG_MAX];
};
static struct memory_statistics memory_statistics;

static const char* memory_tag_strings[ZZ_MEMORY_TAG_MAX] = {
    "UNKNOWN      ",
    "GAME         ",
    "DYNAMIC_ARRAY",
};

b8 memory_initialize()
{
    platform_memory_zero(&memory_statistics, sizeof(memory_statistics));
    return TRUE;
}

void memory_deinitialize()
{

}

void* memory_allocate(u64 size, enum memory_tag tag)
{
    if (tag == ZZ_MEMORY_TAG_UNKNOWN)
    {
        ZZ_LOG_WARNING("memory_allocate called with tag ZZ_MEMORY_TAG_UNKNOWN");
    }

    // TODO Memory alignment
    void* block = platform_memory_allocate(size, FALSE);
    platform_memory_zero(block, size);

    memory_statistics.total_bytes_allocated += size;
    memory_statistics.tagged_bytes_allocated[tag] += size;

    return block;
}

void memory_deallocate(void* block, u64 size, enum memory_tag tag)
{
    if (tag == ZZ_MEMORY_TAG_UNKNOWN)
    {
        ZZ_LOG_WARNING("memory_deallocate called with tag ZZ_MEMORY_TAG_UNKNOWN");
    }

    // TODO Memory alignment
    platform_memory_deallocate(block, FALSE);

    memory_statistics.total_bytes_allocated -= size;
    memory_statistics.tagged_bytes_allocated[tag] -= size;
}

void* memory_zero(void* block, u64 size)
{
    return platform_memory_zero(block, size);
}

void* memory_copy(void* destination, const void* source, u64 size)
{
    return platform_memory_copy(destination, source, size);
}

void* memory_set(void* destination, const i32 value, u64 size)
{
    return platform_memory_set(destination, value, size);
}

char* memory_get_usage_string()
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
        if (memory_statistics.tagged_bytes_allocated[i] > (float)gib)
        {
            unit[0] = 'G';
            amount = memory_statistics.tagged_bytes_allocated[i] / (float)gib;
        }
        else if (memory_statistics.tagged_bytes_allocated[i] > (float)mib)
        {
            unit[0] = 'M';
            amount = memory_statistics.tagged_bytes_allocated[i] / (float)mib;
        }
        if (memory_statistics.tagged_bytes_allocated[i] > (float)kib)
        {
            unit[0] = 'K';
            amount = memory_statistics.tagged_bytes_allocated[i] / (float)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)memory_statistics.tagged_bytes_allocated[i];
        }

        offset += snprintf(buffer + offset, 8000, "\n  %s: %.2f%s", memory_tag_strings[i], amount, unit);
    }

    char* output = _strdup(buffer);
    return output;
}
