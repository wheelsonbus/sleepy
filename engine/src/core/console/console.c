#include "console.h"

#include "platform/core/console/console.h"

void console_write(const char* message, u8 color)
{
    platform_console_write(message, color);
}

void console_write_error(const char* message, u8 color)
{
    platform_console_write_error(message, color);
}