#pragma once

#include "zz.h"

struct log
{

};

struct log_config
{

};

ZZ_API b8 log_initialize(struct log_config* config);
ZZ_API void log_deinitialize();

ZZ_API void _log(int level, const char* message, ...);

#if defined(ZZ_DEBUG)
#define ZZ_LOG_FATAL(message, ...) _log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) _log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...) _log(2, message, ##__VA_ARGS__)
#define ZZ_LOG_INFO(message, ...) _log(3, message, ##__VA_ARGS__)
#define ZZ_LOG_DEBUG(message, ...) _log(4, message, ##__VA_ARGS__)
#define ZZ_LOG_TRACE(message, ...) _log(5, message, ##__VA_ARGS__)

#elif defined(ZZ_RELEASE)
#define ZZ_LOG_FATAL(message, ...) _log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) _log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...)
#define ZZ_LOG_INFO(message, ...)
#define ZZ_LOG_DEBUG(message, ...)
#define ZZ_LOG_TRACE(message, ...)

#endif