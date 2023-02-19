#pragma once

#include <stdint.h>
#include <stddef.h>

#include "x86.h"
#include "../common.h"

#if defined(x86_64) || defined(x86_32)

SSE_TARGET void fast_memcpy_sse(void* restrict dest, void* restrict src, size_t size);

AVX_TARGET void fast_memcpy_avx(void* restrict dest, void* restrict src, size_t size);

SSE_TARGET void fast_memcpy_nt_sse(void* restrict dest, void* restrict src, size_t size);

AVX_TARGET void fast_memcpy_nt_avx(void* restrict dest, void* restrict src, size_t size);

#endif