#include "zz/assert.h"

#include "zz/log.h"

void _zz_assert_fail(const char* expression, const char* message, const char* file, u32 line)
{
    ZZ_LOG_FATAL("Assertion failed: %s, message: \"%s\", file: %s, line: %d", expression, message, file, line);
}