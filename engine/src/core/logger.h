#pragma once

#include "definitions.h"

b8 initialize_logger();
void deinitialize_logger();

ZZ_API void zz_log(int level, const char* message, ...);

#if defined(ZZ_RELEASE)
#define ZZ_LOG_FATAL(message, ...) zz_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) zz_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...)
#define ZZ_LOG_INFO(message, ...)
#define ZZ_LOG_DEBUG(message, ...)
#define ZZ_LOG_TRACE(message, ...)
#elif defined(ZZ_DEBUG)
#define ZZ_LOG_FATAL(message, ...) zz_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) zz_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...) zz_log(2, message, ##__VA_ARGS__)
#define ZZ_LOG_INFO(message, ...) zz_log(3, message, ##__VA_ARGS__)
#define ZZ_LOG_DEBUG(message, ...) zz_log(4, message, ##__VA_ARGS__)
#define ZZ_LOG_TRACE(message, ...) zz_log(5, message, ##__VA_ARGS__)
#endif