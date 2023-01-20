#include "asserter.h"

#include "core/log/log.h"

void asserter_report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    ZZ_LOG_FATAL("Assertion failed: %s, message: \"%s\", file: %s, line: %d", expression, message, file, line);
}