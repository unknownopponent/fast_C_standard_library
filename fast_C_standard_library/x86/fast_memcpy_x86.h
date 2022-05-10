#pragma once

#include <stdint.h>

#include "x86.h"

#if defined(x86_64) || defined(x86_32)

void fast_memcpy_runtime_select(void* dest, void* src, size_t size);

void fast_memcpy_sse(void* dest, void* src, size_t size);

void fast_memcpy_avx(void* dest, void* src, size_t size);

#endif