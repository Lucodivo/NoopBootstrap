#pragma once

#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef s32 b32;

typedef size_t memory_index;

// Differentiating between statics
#define local_persist static  // persisting function variable
#define internal static // file private function
#define global static // global variables

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define Pi32 3.14159265359f
#define PiOverTwo32 1.57079632679f
#define Tau32 6.28318530717958647692f
#define RadiansPerDegree (Pi32 / 180.0f)
#define Radians(x) (x * RadiansPerDegree)
#define U32_MAX ~0u

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define Min(x, y) (x <= y ? x : y)
#define Max(x, y) (x >= y ? x : y)
#define Clamp(x, low, high) (x <= low ? low : (x >= high) ? high : x)

struct ivec2 {
  s32 x;
  s32 y;
};