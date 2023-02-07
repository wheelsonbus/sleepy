#ifndef ZZ_LOG_H
#define ZZ_LOG_H

#include "zz.h"

struct zz_log
{

};

struct zz_log_config
{

};

ZZ_API b8 zz_log_initialize(struct zz_log_config* config);
ZZ_API void zz_log_deinitialize();

ZZ_API void _zz_log(int level, const char* message, ...);

#if defined(ZZ_DEBUG)
#define ZZ_LOG_FATAL(message, ...) _zz_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) _zz_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...) _zz_log(2, message, ##__VA_ARGS__)
#define ZZ_LOG_INFO(message, ...) _zz_log(3, message, ##__VA_ARGS__)
#define ZZ_LOG_DEBUG(message, ...) _zz_log(4, message, ##__VA_ARGS__)
#define ZZ_LOG_TRACE(message, ...) _zz_log(5, message, ##__VA_ARGS__)
#elif defined(ZZ_RELEASE)
#define ZZ_LOG_FATAL(message, ...) _zz_log(0, message, ##__VA_ARGS__)
#define ZZ_LOG_ERROR(message, ...) _zz_log(1, message, ##__VA_ARGS__)
#define ZZ_LOG_WARNING(message, ...)
#define ZZ_LOG_INFO(message, ...)
#define ZZ_LOG_DEBUG(message, ...)
#define ZZ_LOG_TRACE(message, ...)
#endif

#endif