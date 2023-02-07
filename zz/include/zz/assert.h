#ifndef ZZ_ASSERT_H
#define ZZ_ASSERT_H

#include "zz.h"

ZZ_API void _assert_fail(const char* expression, const char* message, const char* file, u32 line);

#if defined(ZZ_DEBUG)
#if _MSC_VER
#include <intrin.h>
#define ZZ_ASSERT_DEBUG_BREAK() __debugbreak()
#else
#define ZZ_ASSERT_DEBUG_BREAK() __builtin_trap()
#endif
#define ZZ_ASSERT(expression) {if (expression) {} else {_assert_fail(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_MESSAGE(expression, message) {if (expression) {} else {_assert_fail(#expression, message, __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_DEBUG(expression) {if (expression) {} else {_assert_fail(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#elif defined(ZZ_RELEASE)
#if _MSC_VER
#include <intrin.h>
#define ZZ_ASSERT_DEBUG_BREAK() __debugbreak()
#else
#define ZZ_ASSERT_DEBUG_BREAK() __builtin_trap()
#endif
#define ZZ_ASSERT(expression) {if (expression) {} else {_assert_fail(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_MESSAGE(expression, message) {if (expression) {} else {_assert_fail(#expression, message, __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_DEBUG(expression)
#endif

#endif