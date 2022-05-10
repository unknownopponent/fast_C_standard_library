
#include "fast_memcpy_x86.h"
#include "../fast_memcpy.h"

#if defined(x86_64) || defined(x86_32)

#include <immintrin.h>

void(*fast_memcpy_runtime)(void*, void*, size_t) = fast_memcpy_runtime_select;

void fast_memcpy_runtime_select(void* dest, void* src, size_t size)
{
	if (cpuid_sse())
	{
		fast_memcpy_runtime = fast_memcpy_sse;
		fast_memcpy_runtime(dest, src, size);
		return;
	}
	fast_memcpy_runtime = fast_memcpy;
	fast_memcpy_runtime(dest, src, size);
	return;
}

void fast_memcpy_sse(void* dest, void* src, size_t size)
{
	__m128 tmp, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

	#if defined(x86_64)
	__m128 tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15, tmp16;
	
	while (size > 256)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((uint64_t*)src + 2);
		tmp3 = _mm_loadu_ps((uint64_t*)src + 4);
		tmp4 = _mm_loadu_ps((uint64_t*)src + 6);
		tmp5 = _mm_loadu_ps((uint64_t*)src + 8);
		tmp6 = _mm_loadu_ps((uint64_t*)src + 10);
		tmp7 = _mm_loadu_ps((uint64_t*)src + 12);
		tmp8 = _mm_loadu_ps((uint64_t*)src + 14);
		tmp9 = _mm_loadu_ps((uint64_t*)src + 16);
		tmp10 = _mm_loadu_ps((uint64_t*)src + 18);
		tmp11 = _mm_loadu_ps((uint64_t*)src + 20);
		tmp12 = _mm_loadu_ps((uint64_t*)src + 22);
		tmp13 = _mm_loadu_ps((uint64_t*)src + 24);
		tmp14 = _mm_loadu_ps((uint64_t*)src + 26);
		tmp15 = _mm_loadu_ps((uint64_t*)src + 28);
		tmp16 = _mm_loadu_ps((uint64_t*)src + 30);
		_mm_storeu_ps(dest, tmp);
		_mm_storeu_ps((uint64_t*)dest + 2, tmp2);
		_mm_storeu_ps((uint64_t*)dest + 4, tmp3);
		_mm_storeu_ps((uint64_t*)dest + 6, tmp4);
		_mm_storeu_ps((uint64_t*)dest + 8, tmp5);
		_mm_storeu_ps((uint64_t*)dest + 10, tmp6);
		_mm_storeu_ps((uint64_t*)dest + 12, tmp7);
		_mm_storeu_ps((uint64_t*)dest + 14, tmp8);
		_mm_storeu_ps((uint64_t*)dest + 16, tmp9);
		_mm_storeu_ps((uint64_t*)dest + 18, tmp10);
		_mm_storeu_ps((uint64_t*)dest + 20, tmp11);
		_mm_storeu_ps((uint64_t*)dest + 22, tmp12);
		_mm_storeu_ps((uint64_t*)dest + 24, tmp13);
		_mm_storeu_ps((uint64_t*)dest + 26, tmp14);
		_mm_storeu_ps((uint64_t*)dest + 28, tmp15);
		_mm_storeu_ps((uint64_t*)dest + 30, tmp16);
		dest = (uint64_t*)dest + 32;
		src = (uint64_t*)src + 32;
		size -= 256;
	}
	#endif

	while (size > 128)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((uint64_t*)src + 2);
		tmp3 = _mm_loadu_ps((uint64_t*)src + 4);
		tmp4 = _mm_loadu_ps((uint64_t*)src + 6);
		tmp5 = _mm_loadu_ps((uint64_t*)src + 8);
		tmp6 = _mm_loadu_ps((uint64_t*)src + 10);
		tmp7 = _mm_loadu_ps((uint64_t*)src + 12);
		tmp8 = _mm_loadu_ps((uint64_t*)src + 14);
		_mm_storeu_ps(dest, tmp);
		_mm_storeu_ps((uint64_t*)dest + 2, tmp2);
		_mm_storeu_ps((uint64_t*)dest + 4, tmp3);
		_mm_storeu_ps((uint64_t*)dest + 6, tmp4);
		_mm_storeu_ps((uint64_t*)dest + 8, tmp5);
		_mm_storeu_ps((uint64_t*)dest + 10, tmp6);
		_mm_storeu_ps((uint64_t*)dest + 12, tmp7);
		_mm_storeu_ps((uint64_t*)dest + 14, tmp8);
		dest = (uint64_t*)dest + 16;
		src = (uint64_t*)src + 16;
		size -= 128;
	}
	while (size > 64)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((uint64_t*)src + 2);
		tmp3 = _mm_loadu_ps((uint64_t*)src + 4);
		tmp4 = _mm_loadu_ps((uint64_t*)src + 6);
		_mm_storeu_ps(dest, tmp);
		_mm_storeu_ps((uint64_t*)dest + 2, tmp2);
		_mm_storeu_ps((uint64_t*)dest + 4, tmp3);
		_mm_storeu_ps((uint64_t*)dest + 6, tmp4);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 32)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((uint64_t*)src + 2);
		_mm_storeu_ps(dest, tmp);
		_mm_storeu_ps((uint64_t*)dest + 2, tmp2);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 16)
	{
		tmp = _mm_loadu_ps(src);
		_mm_storeu_ps(dest, tmp);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 8)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 4)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 2)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 1)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

void fast_memcpy_avx(void* dest, void* src, size_t size)
{
	__m256 tmp, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15, tmp16;
	__m128 tmp128;

	while (size > 512)
	{
		tmp = _mm256_loadu_ps(src);
		tmp2 = _mm256_loadu_ps((uint64_t*)src + 4);
		tmp3 = _mm256_loadu_ps((uint64_t*)src + 8);
		tmp4 = _mm256_loadu_ps((uint64_t*)src + 12);
		tmp5 = _mm256_loadu_ps((uint64_t*)src + 16);
		tmp6 = _mm256_loadu_ps((uint64_t*)src + 20);
		tmp7 = _mm256_loadu_ps((uint64_t*)src + 24);
		tmp8 = _mm256_loadu_ps((uint64_t*)src + 28);
		tmp9 = _mm256_loadu_ps((uint64_t*)src + 32);
		tmp10 = _mm256_loadu_ps((uint64_t*)src + 36);
		tmp11 = _mm256_loadu_ps((uint64_t*)src + 40);
		tmp12 = _mm256_loadu_ps((uint64_t*)src + 44);
		tmp13 = _mm256_loadu_ps((uint64_t*)src + 48);
		tmp14 = _mm256_loadu_ps((uint64_t*)src + 52);
		tmp15 = _mm256_loadu_ps((uint64_t*)src + 56);
		tmp16 = _mm256_loadu_ps((uint64_t*)src + 60);
		_mm256_storeu_ps(dest, tmp);
		_mm256_storeu_ps((uint64_t*)dest + 4, tmp2);
		_mm256_storeu_ps((uint64_t*)dest + 8, tmp3);
		_mm256_storeu_ps((uint64_t*)dest + 12, tmp4);
		_mm256_storeu_ps((uint64_t*)dest + 16, tmp5);
		_mm256_storeu_ps((uint64_t*)dest + 20, tmp6);
		_mm256_storeu_ps((uint64_t*)dest + 24, tmp7);
		_mm256_storeu_ps((uint64_t*)dest + 28, tmp8);
		_mm256_storeu_ps((uint64_t*)dest + 32, tmp9);
		_mm256_storeu_ps((uint64_t*)dest + 36, tmp10);
		_mm256_storeu_ps((uint64_t*)dest + 40, tmp11);
		_mm256_storeu_ps((uint64_t*)dest + 44, tmp12);
		_mm256_storeu_ps((uint64_t*)dest + 48, tmp13);
		_mm256_storeu_ps((uint64_t*)dest + 52, tmp14);
		_mm256_storeu_ps((uint64_t*)dest + 56, tmp15);
		_mm256_storeu_ps((uint64_t*)dest + 60, tmp16);
		dest = (uint64_t*)dest + 64;
		src = (uint64_t*)src + 64;
		size -= 512;
	}
	while (size > 256)
	{
		tmp = _mm256_loadu_ps(src);
		tmp2 = _mm256_loadu_ps((uint64_t*)src + 4);
		tmp3 = _mm256_loadu_ps((uint64_t*)src + 8);
		tmp4 = _mm256_loadu_ps((uint64_t*)src + 12);
		tmp5 = _mm256_loadu_ps((uint64_t*)src + 16);
		tmp6 = _mm256_loadu_ps((uint64_t*)src + 20);
		tmp7 = _mm256_loadu_ps((uint64_t*)src + 24);
		tmp8 = _mm256_loadu_ps((uint64_t*)src + 28);
		_mm256_storeu_ps(dest, tmp);
		_mm256_storeu_ps((uint64_t*)dest + 4, tmp2);
		_mm256_storeu_ps((uint64_t*)dest + 8, tmp3);
		_mm256_storeu_ps((uint64_t*)dest + 12, tmp4);
		_mm256_storeu_ps((uint64_t*)dest + 16, tmp5);
		_mm256_storeu_ps((uint64_t*)dest + 20, tmp6);
		_mm256_storeu_ps((uint64_t*)dest + 24, tmp7);
		_mm256_storeu_ps((uint64_t*)dest + 28, tmp8);
		dest = (uint64_t*)dest + 32;
		src = (uint64_t*)src + 32;
		size -= 256;
	}
	while (size > 128)
	{
		tmp = _mm256_loadu_ps(src);
		tmp2 = _mm256_loadu_ps((uint64_t*)src + 4);
		tmp3 = _mm256_loadu_ps((uint64_t*)src + 8);
		tmp4 = _mm256_loadu_ps((uint64_t*)src + 12);
		_mm256_storeu_ps(dest, tmp);
		_mm256_storeu_ps((uint64_t*)dest + 4, tmp2);
		_mm256_storeu_ps((uint64_t*)dest + 8, tmp3);
		_mm256_storeu_ps((uint64_t*)dest + 12, tmp4);
		dest = (uint64_t*)dest + 16;
		src = (uint64_t*)src + 16;
		size -= 128;
	}
	while (size > 64)
	{
		tmp = _mm256_loadu_ps(src);
		tmp = _mm256_loadu_ps((uint64_t*)src + 4);
		_mm256_storeu_ps(dest, tmp);
		_mm256_storeu_ps((uint64_t*)dest + 4, tmp);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 32)
	{
		tmp = _mm256_loadu_ps(src);
		_mm256_storeu_ps(dest, tmp);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 16)
	{
		tmp128 = _mm_loadu_ps(src);
		_mm_storeu_ps(dest, tmp128);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 8)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 4)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 2)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 1)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

#endif