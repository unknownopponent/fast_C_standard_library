
#pragma once

#include <stdint.h>
#include <stddef.h>

#include "x86.h"

#if defined(x86_64) || defined(x86_32)

SSE_TARGET void* fast_memmove_sse(void* destination, void* source, size_t size);

AVX_TARGET void* fast_memmove_avx(void* destination, void* source, size_t size);

SSE_TARGET void* fast_memmove_sse_nt(void* destination, void* source, size_t size);

AVX_TARGET void* fast_memmove_avx_nt(void* destination, void* source, size_t size);

#endif