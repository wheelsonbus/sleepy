#pragma once

#define ZZ_VERSION_MAJOR 1
#define ZZ_VERSION_MINOR 0
#define ZZ_VERSION_PATCH 0

// Unsigned integer types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed integer types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean types
typedef char b8;
typedef int b32;

// Boolean values
#define ZZ_TRUE 1
#define ZZ_FALSE 0

#define ZZ_NULL (void*)0

// Utility macros
#define ZZ_UTILITY_CLAMP(value, min, max) (value <= min) ? min : ((value >= max) ? max : value)

// Static assertion macro
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// Type assertions
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
STATIC_ASSERT(sizeof(b8) == 1, "Expected b8 to be 1 byte.");
STATIC_ASSERT(sizeof(b32) == 4, "Expected b32 to be 4 bytes.");

#ifdef ZZ_EXPORT
// Exports
#ifdef _MSC_VER
#define ZZ_API __declspec(dllexport)
#else
#define ZZ_API __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define ZZ_API __declspec(dllimport)
#else
#define ZZ_API
#endif
#endif

// Inlining
#ifdef _MSC_VER
#define ZZ_INLINE __forceinline
#define ZZ_NOINLINE __declspec(noinline)
#else
#define ZZ_INLINE static inline
#define ZZ_NOINLINE
#endif