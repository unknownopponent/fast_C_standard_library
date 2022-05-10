#pragma once

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

extern uint64_t cpuid_sse();
extern uint64_t cpuid_avx();