#ifdef ZZ_WINDOWS

#include <Windows.h>
#include <stdio.h>

#include "zz/internal/filesystem.h"
#include "filesystem.h"

#include "zz/log.h"
#include "zz/memory.h"

b8 zz_internal_filesystem_initialize(struct zz_internal_filesystem_config* config)
{
    return ZZ_TRUE;
}

void zz_internal_filesystem_deinitialize()
{
    
}

u64 zz_internal_filesystem_get_size(char* path)
{
    LARGE_INTEGER size;

    HANDLE hFile = CreateFile(path, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        ZZ_LOG_ERROR("Failed to create file handle for path %s.", path);
        return 0;
    }

    if (!GetFileSizeEx(hFile, &size))
    {
        ZZ_LOG_ERROR("Failed to get file size for path %s.", path);
        return 0;
    }

    CloseHandle(hFile);

    return (u64)size.QuadPart;
}

void zz_internal_filesystem_load(char* path, void* destination, u64 size)
{
    HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        ZZ_LOG_ERROR("Failed to create file handle for path %s.", path);
    }

    DWORD nNumberOfBytesRead = 0;
    if (!ReadFile(hFile, destination, size, &nNumberOfBytesRead, NULL))
    {
        ZZ_LOG_ERROR("Failed to read file at path %s.", path);
    }

    CloseHandle(hFile);
}

b8 zz_filesystem_open(char* path, void** file)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        ZZ_LOG_ERROR("Failed to create file handle for path %s.", path);
    }

    *file = (void*)f;

    return ZZ_TRUE;
}

void zz_filesystem_close(void* file)
{
    FILE* f = (FILE*)file;
    fclose(f);
}

b8 zz_filesystem_read_line(void* file, char* line, u64 max)
{
    if (fgets(line, max, (FILE*)file) != line)
    {
        return ZZ_FALSE;
    }
    line[strcspn(line, "\n")] = '\0';

    return ZZ_TRUE;
}

#endif