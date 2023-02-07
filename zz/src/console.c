#include "zz/console.h"

#include "internal/console.h"

void console_write(const char* message, u8 color)
{
    internal_console_write(message, color);
}

void console_write_error(const char* message, u8 color)
{
    internal_console_write_error(message, color);
}