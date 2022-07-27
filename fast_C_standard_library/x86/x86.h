#pragma once

#include <stdint.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__amd64) || defined(__amd64__) || defined (__x86_64) || defined(__x86_64__)
	#define  x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#define x86_32
#endif

#if (_M_IX86_FP == 1) || defined(__SSE__)
	#define SSE
#endif

#if (_M_IX86_FP == 2) || defined(__SSE2__)
	#define SSE2
#endif

#if defined(__AVX__)
	#define AVX
#endif

#if defined(__AVX2__)
	#define AVX2
#endif

#if defined(x86_64) || defined(x86_32)
extern uint64_t cpuid_sse();

extern uint64_t cpuid_avx();
#endif

#if defined(__GNUC__) || defined(__clang__)

#define SSE_TARGET __attribute__ ((__target__ ("sse")))
#define SSE2_TARGET __attribute__ ((__target__ ("sse2")))
#define SSE3_TARGET __attribute__ ((__target__ ("sse3")))
#define SSE41_TARGET __attribute__ ((__target__ ("sse4.1")))
#define SSE42_TARGET __attribute__ ((__target__ ("sse4.2")))

#define AVX_TARGET __attribute__ ((__target__ ("avx")))
#define AVX2_TARGET __attribute__ ((__target__ ("avx2")))

#else

#define SSE_TARGET
#define SSE2_TARGET
#define SSE3_TARGET
#define SSE41_TARGET 
#define SSE42_TARGET 

#define AVX_TARGET 
#define AVX2_TARGET 

#endif