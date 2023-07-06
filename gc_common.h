/* date = October 12th 2020 6:33 pm */

#include <stdint.h>
#include <float.h>
#include <limits.h>

#ifndef COMMON_H
#define COMMON_H

#define internal static
#define local_persist static
#define global static

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;
typedef s16 b16;
typedef s8 b8;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define S8_MIN (-128)
#define S16_MIN (-32768)
#define S32_MIN (-2147483648LL)
#define S64_MIN (-9223372036854775808LL)

#define U8_MAX 0xffU
#define S8_MAX 0x7f
#define U16_MAX 0xffffU
#define S16_MAX 0x7fff
#define U32_MAX 0xffffffffU
#define S32_MAX 0x7fffffff
#define U64_MAX 0xffffffffffffffffULL
#define S64_MAX 0x7fffffffffffffffLL

#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX

#define KILOBYTES(value) ((value) * 1024)
#define MEGABYTES(value) (KILOBYTES(value) * 1024ULL)
#define GIGABYTES(value) (MEGABYTES(value) * 1024ULL)
#define TERABYTES(value) (GIGABYTES(value) * 1024ULL)

#define THOUSAND(a) ((a) * 1000)
#define MILLION(a) (THOUSAND(a) * 1000)
#define BILLION(a) (MILLION(a) * 1000ULL)

#define ARRAYCOUNT(array) (sizeof(array) / sizeof((array)[0]))

#ifdef GC_DEBUG
#define ASSERT(expression) do {if (!(expression)) (*(volatile int *)0) = 0; } while(0)
#else
#define ASSERT(expression)
#endif

// #define MIN(a, b) ((a) > (b) ? (b) : (a))
// #define MAX(a, b) ((a) < (b) ? (b) : (a))
#define ABS(a) ((a) < 0 ? -(a) : (a))
#define CLAMP(min, val, max) (MAX(MIN(val, max), min))
#define ROUND_UP_BY_POWER_OF_2(value, powerOf2) (((value) + (powerOf2) - 1) & ~((powerOf2) - 1))
#define ROUND_DOWN_BY_POWER_OF_2(value, powerOf2) ((value) & ~((powerOf2) - 1))

#define MERGE_(a,b)  a##b
#define MERGE(a,b)  MERGE_(a, b)
#define LABEL_(a) MERGE_(unique_name_, a)
#define UNIQUE_NAME LABEL_(__LINE__)
#define STRINGISE(s) #s

#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

#endif //COMMON_H
