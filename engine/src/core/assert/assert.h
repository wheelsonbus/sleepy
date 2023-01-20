#pragma once

#include "zz.h"

#if defined(ZZ_DEBUG)
#if _MSC_VER
#include <intrin.h>
#define ZZ_ASSERT_DEBUG_BREAK() __debugbreak()
#else
#define ZZ_ASSERT_DEBUG_BREAK() __builtin_trap()
#endif

ZZ_API void assert_report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define ZZ_ASSERT(expression) {if (expression) {} else {assert_report_assertion_failure(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_MESSAGE(expression, message) {if (expression) {} else {assert_report_assertion_failure(#expression, message, __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_DEBUG(expression) {if (expression) {} else {assert_report_assertion_failure(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}

#elif defined(ZZ_RELEASE)
#if _MSC_VER
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

ZZ_API void assert_report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define ZZ_ASSERT(expression) {if (expression) {} else {assert_report_assertion_failure(#expression, "", __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_MESSAGE(expression, message) {if (expression) {} else {assert_report_assertion_failure(#expression, message, __FILE__, __LINE__); ZZ_ASSERT_DEBUG_BREAK();}}
#define ZZ_ASSERT_DEBUG(expression)

#endif