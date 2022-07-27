#pragma once

#include <stdint.h>
#include <string.h>

#include "x86/fast_memcpy_x86.h"

extern volatile void (*fast_memcpy_runtime)(void* dest, void* src, size_t size);

extern volatile void (*fast_memcpy_nt_runtime)(void* dest, void* src, size_t size);


void fast_memcpy(void* dest, void* src, size_t size);

/* FAST_MEMCPY macro */

#if defined(NO_ASM)
	#define FAST_MEMCPY fast_memcpy
	#define FAST_MEMCPY_NT memcpy
#elif defined(USE_AVX)
	#define FAST_MEMCPY fast_memcpy_avx
	#define FAST_MEMCPY_NT fast_memcpy_nt_avx
#elif defined(USE_SSE)
	#define FAST_MEMCPY fast_memcpy_sse
	#define FAST_MEMCPY_NT fast_memcpy_nt_sse
#else
	#if defined(x86_64) || defined(x86_32)
		#define FAST_MEMCPY fast_memcpy_runtime
		#define FAST_MEMCPY_NT fast_memcpy_nt_runtime
	#else
		#define FAST_MEMCPY fast_memcpy
		#define FAST_MEMCPY_NT memcpy
	#endif
#endif