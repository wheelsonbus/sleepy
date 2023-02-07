#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#define PLAYGROUND_VERSION_MAJOR 1
#define PLAYGROUND_MINOR 0
#define PLAYGROUND_PATCH 0

#ifdef PLAYGROUND_EXPORT
#ifdef _MSC_VER
#define PLAYGROUND_API __declspec(dllexport)
#else
#define PLAYGROUND_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define PLAYGROUND_API __declspec(dllimport)
#else
#define PLAYGROUND_API
#endif
#endif

#endif