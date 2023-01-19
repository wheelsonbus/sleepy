#include "asserter.h"

void zz_report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    zz_log(0, "Assertion failed: %s, message: \"%s\", file: %s, line: %d", expression, message, file, line);
}