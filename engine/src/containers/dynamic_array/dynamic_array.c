#include "dynamic_array.h"

#include "core/memory/memory.h"
#include "core/logger/logger.h"

ZZ_API void* _dynamic_array_create(u64 capacity, u64 stride)
{
    u64 header_size = ZZ_DYNAMIC_ARRAY_FIELD_MAX * sizeof(u64);
    u64 array_size = capacity * stride;
    u64* dynamic_array = memory_allocate(header_size + array_size, ZZ_MEMORY_TAG_DYNAMIC_ARRAY);
    memory_set(dynamic_array, 0, header_size + array_size);
    dynamic_array[ZZ_DYNAMIC_ARRAY_FIELD_CAPACITY] = capacity;
    dynamic_array[ZZ_DYNAMIC_ARRAY_FIELD_LENGTH] = 0;
    dynamic_array[ZZ_DYNAMIC_ARRAY_FIELD_STRIDE] = stride;
    return (void*)(dynamic_array + ZZ_DYNAMIC_ARRAY_FIELD_MAX);
}

ZZ_API void _dynamic_array_destroy(void* dynamic_array)
{
    u64* header = (u64*)dynamic_array - ZZ_DYNAMIC_ARRAY_FIELD_MAX;
    u64 header_size = ZZ_DYNAMIC_ARRAY_FIELD_MAX * sizeof(u64);
    u64 total_size = header_size + (header[ZZ_DYNAMIC_ARRAY_FIELD_CAPACITY] * header[ZZ_DYNAMIC_ARRAY_FIELD_STRIDE]);
    memory_deallocate(header, total_size, ZZ_MEMORY_TAG_DYNAMIC_ARRAY);
}

ZZ_API u64 _dynamic_array_get_field(void* dynamic_array, u64 field)
{
    u64* header = (u64*)dynamic_array - ZZ_DYNAMIC_ARRAY_FIELD_MAX;
    return header[field];
}

ZZ_API void _dynamic_array_set_field(void* dynamic_array, u64 field, u64 value)
{
    u64* header = (u64*)dynamic_array - ZZ_DYNAMIC_ARRAY_FIELD_MAX;
    header[field] = value;
}

ZZ_API void* _dynamic_array_grow(void* dynamic_array)
{
    u64 length = dynamic_array_get_length(dynamic_array);
    u64 stride = dynamic_array_get_stride(dynamic_array);
    void* new_dynamic_array = _dynamic_array_create(ZZ_DYNAMIC_ARRAY_RESIZE_FACTOR * dynamic_array_get_capacity(dynamic_array), stride);
    memory_copy(new_dynamic_array, dynamic_array, length * stride);
    _dynamic_array_set_field(new_dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, length);
    _dynamic_array_destroy(dynamic_array);
    return new_dynamic_array;
}

ZZ_API void* _dynamic_array_push(void* dynamic_array, const void* source)
{
    u64 length = dynamic_array_get_length(dynamic_array);
    u64 stride = dynamic_array_get_stride(dynamic_array);
    if (length >= dynamic_array_get_capacity(dynamic_array))
    {
        dynamic_array = _dynamic_array_grow(dynamic_array);
    }

    void* end = (void*)((u64)dynamic_array + (length * stride));
    memory_copy(end, source, stride);
    _dynamic_array_set_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, length + 1);
    return dynamic_array;
}

ZZ_API void* _dynamic_array_pop(void* dynamic_array, void* destination)
{
    u64 length = dynamic_array_get_length(dynamic_array);
    u64 stride = dynamic_array_get_stride(dynamic_array);
    void* last = (void*)((u64)dynamic_array + ((length - 1) * stride));
    memory_copy(destination, last, stride);
    _dynamic_array_set_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, length - 1);
    return dynamic_array;
}

ZZ_API void* _dynamic_array_insert_at(void* dynamic_array, u64 index, const void* source)
{
    u64 length = dynamic_array_get_length(dynamic_array);
    u64 stride = dynamic_array_get_stride(dynamic_array);
    if (index >= length)
    {
        ZZ_LOG_ERROR("Tried to insert into dynamic array at invalid index. Length: %i, index: %i", length, index);
        return dynamic_array;
    }
    if (length >= dynamic_array_get_capacity(dynamic_array))
    {
        dynamic_array = _dynamic_array_grow(dynamic_array);
    }
    
    u64 address = (u64)dynamic_array;
    void* element = (void*)(address + (index * stride));
    if (index != length - 1)
    {
        memory_copy((void*)(address + ((index + 1) * stride)), element, stride * (length - index));
    }
    memory_copy(element, source, stride);

    _dynamic_array_set_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, length + 1);
    return dynamic_array;
}

ZZ_API void* _dynamic_array_pop_at(void* dynamic_array, u64 index, void* destination)
{
    u64 length = dynamic_array_get_length(dynamic_array);
    u64 stride = dynamic_array_get_stride(dynamic_array);
    if (index >= length)
    {
        ZZ_LOG_ERROR("Tried to pop from dynamic array at invalid index. Length: %i, index: %i", length, index);
        return dynamic_array;
    }

    u64 address = (u64)dynamic_array;
    void* element = (void*)(address + (index * stride));
    memory_copy(destination, element, stride);
    if (index != length - 1)
    {
        memory_copy(element, (void*)(address + ((index + 1) * stride)), stride * (length - index));
    }

    _dynamic_array_set_field(dynamic_array, ZZ_DYNAMIC_ARRAY_FIELD_LENGTH, length - 1);
    return dynamic_array;
}