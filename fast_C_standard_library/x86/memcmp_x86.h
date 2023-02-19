
#pragma once

#include "x86.h"

SSE2_TARGET int memcmp_sse2(void* ptr1, void* ptr2, size_t num);

AVX2_TARGET int memcmp_avx2(void* ptr1, void* ptr2, size_t num);