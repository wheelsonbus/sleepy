#pragma once

#include "zz.h"

ZZ_API b8 logger_initialize();
ZZ_API void logger_deinitialize();

ZZ_API void logger_log(int level, const char* message, ...);

#if defined(ZZ_DEBUG)
#define ZZ_LOG_FATAL(message, ...) logger_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) logger_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...) logger_log(2, message, ##__VA_ARGS__)
#define ZZ_LOG_INFO(message, ...) logger_log(3, message, ##__VA_ARGS__)
#define ZZ_LOG_DEBUG(message, ...) logger_log(4, message, ##__VA_ARGS__)
#define ZZ_LOG_TRACE(message, ...) logger_log(5, message, ##__VA_ARGS__)

#elif defined(ZZ_RELEASE)
#define ZZ_LOG_FATAL(message, ...) logger_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) logger_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...)
#define ZZ_LOG_INFO(message, ...)
#define ZZ_LOG_DEBUG(message, ...)
#define ZZ_LOG_TRACE(message, ...)

#endif