#pragma once

#include <stdint.h>

#include "x86/fast_memcpy_x86.h"

static void (*fast_memcpy_runtime)(void* dest, void* src, size_t size) =
#if defined(x86_64) || defined(x86_32)
	fast_memcpy_runtime_select;
#else
	fast_memcpy;
#endif

void fast_memcpy(void* dest, void* src, size_t size);

/* FAST_MEMCPY macro */

#if defined(NO_ASM)
#define FAST_MEMCPY fast_memcpy
#elif defined(SSE) || defined(SSE2) || defined(AVX) || defined(AVX2) /* AVX version seems not to be faster from my testing and may cause downclock */
#define FAST_MEMCPY fast_memcpy_sse
#else
#define FAST_MEMCPY fast_memcpy_runtime
#endif