#include "zz/log.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "zz/console.h"

b8 zz_log_initialize(struct zz_log_config* config) {
    ZZ_LOG_INFO("Log module initialized.");
    return ZZ_TRUE;
}

void zz_log_deinitialize() {
    ZZ_LOG_INFO("Log module deinitialized.");
}

void _zz_log(int level, const char* message, ...)
{
    const char* level_prefixes[6] = {"[FATAL] ", "[ERROR] ", "[WARNING] ", "[INFO] ", "[DEBUG] ", "[TRACE] "};
    
    char formatted_message[32000];
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(formatted_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char output[32000];
    sprintf(output, "%s%s\n", level_prefixes[level], formatted_message);

    if (level < 2)
    {
        zz_console_write_error(output, level);
    }
    else 
    {
        zz_console_write(output, level);
    }
}