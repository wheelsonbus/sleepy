#ifdef ZZ_WINDOWS

#include "zz/internal/console.h"

#include <windows.h>

void zz_internal_console_write(const char* message, u8 color)
{   
    const u8 level_colors[6] = {64, 4, 6, 2, 1, 8};

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, level_colors[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD bytesWritten = 0;
    WriteConsoleA(consoleHandle, message, (DWORD)length, bytesWritten, 0);
}
void zz_internal_console_write_error(const char* message, u8 color)
{   
    const u8 level_colors[6] = {64, 4, 6, 2, 1, 8};

    HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(consoleHandle, level_colors[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD bytesWritten = 0;
    WriteConsoleA(consoleHandle, message, (DWORD)length, bytesWritten, 0);
}

#endif