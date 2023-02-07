#ifndef ZZ_INTERNAL_CONSOLE_H
#define ZZ_INTERNAL_CONSOLE_H

#include "zz/zz.h"

void internal_console_write(const char* message, u8 color);
void internal_console_write_error(const char* message, u8 color);

#endif