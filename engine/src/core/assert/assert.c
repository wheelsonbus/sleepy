#include "assert.h"

#include "core/log/log.h"

void _assert_fail(const char* expression, const char* message, const char* file, i32 line)
{
    ZZ_LOG_FATAL("Assertion failed: %s, message: \"%s\", file: %s, line: %d", expression, message, file, line);
}