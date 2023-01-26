#include "zz/string.h"

#include <string.h>

#include "zz/memory.h"

u64 string_get_length(const char* string)
{
    return strlen(string);
}

char* string_copy(const char* string)
{
    u64 length = strlen(string);
    char* copy = memory_allocate(length + 1, ZZ_MEMORY_TAG_STRING);
    memory_copy(copy, string, length + 1);
    return copy;
}

b8 string_equals(const char* string0, const char* string1)
{
    return strcmp(string0, string1) == 0;
}