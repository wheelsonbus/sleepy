#include "zz/console.h"

#include "zz/internal/console.h"

void zz_console_write(const char* message, u8 color)
{
    zz_internal_console_write(message, color);
}

void zz_console_write_error(const char* message, u8 color)
{
    zz_internal_console_write_error(message, color);
}