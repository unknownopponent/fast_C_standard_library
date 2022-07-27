
#include "fast_memcpy_x86.h"
#include "../fast_memcpy.h"

#if defined(x86_64) || defined(x86_32)

#include <immintrin.h>

volatile void(*fast_memcpy_runtime)(void*, void*, size_t) = fast_memcpy_runtime_select;

void fast_memcpy_runtime_select(void* dest, void* src, size_t size)
{
	if (cpuid_avx())
	{
		fast_memcpy_runtime = fast_memcpy_avx;
		fast_memcpy_runtime(dest, src, size);
		return;
	}
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

volatile void(*fast_memcpy_nt_runtime)(void*, void*, size_t) = fast_memcpy_nt_runtime_select;

void fast_memcpy_nt_runtime_select(void* dest, void* src, size_t size)
{
	if (cpuid_avx())
	{
		fast_memcpy_nt_runtime = fast_memcpy_nt_avx;
		fast_memcpy_nt_runtime(dest, src, size);
		return;
	}
	if (cpuid_sse())
	{
		fast_memcpy_nt_runtime = fast_memcpy_nt_sse;
		fast_memcpy_nt_runtime(dest, src, size);
		return;
	}
	fast_memcpy_nt_runtime = (void(*)(void*, void*, size_t))memcpy;
	fast_memcpy_nt_runtime(dest, src, size);
	return;
}

SSE_TARGET void fast_memcpy_sse(void* dest, void* src, size_t size)
{
	__m128 tmp, tmp2, tmp3, tmp4;
	
	while (size > 63)
	{
		tmp = _mm_loadu_ps((float*)src);
		tmp2 = _mm_loadu_ps((float*)src + 4);
		tmp3 = _mm_loadu_ps((float*)src + 8);
		tmp4 = _mm_loadu_ps((float*)src + 12);
		_mm_storeu_ps((float*)dest, tmp);
		_mm_storeu_ps((float*)dest + 4, tmp2);
		_mm_storeu_ps((float*)dest + 8, tmp3);
		_mm_storeu_ps((float*)dest + 12, tmp4);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 31)
	{
		tmp = _mm_loadu_ps((float*)src);
		tmp2 = _mm_loadu_ps((float*)src + 4);
		_mm_storeu_ps((float*)dest, tmp);
		_mm_storeu_ps((float*)dest + 4, tmp2);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 15)
	{
		tmp = _mm_loadu_ps((float*)src);
		_mm_storeu_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 7)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 3)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 1)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

AVX_TARGET void fast_memcpy_avx(void* dest, void* src, size_t size)
{
	__m256 tmp, tmp2;
	__m128 tmp128;
	
	while (size > 63)
	{
		tmp = _mm256_loadu_ps((float*)src);
		tmp2 = _mm256_loadu_ps((float*)src + 8);
		_mm256_storeu_ps((float*)dest, tmp);
		_mm256_storeu_ps((float*)dest + 8, tmp2);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 31)
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_storeu_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 15)
	{
		tmp128 = _mm_loadu_ps(src);
		_mm_storeu_ps(dest, tmp128);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 7)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 3)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 1)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

SSE_TARGET void fast_memcpy_nt_sse(void* dest, void* src, size_t size)
{
	__m128 tmp, tmp2, tmp3, tmp4;
	
	size_t alignement = 16 - ((size_t)dest % 16);

	fast_memcpy(dest, src, alignement);
	dest = (uint8_t*)dest + alignement;
	src = (uint8_t*)src + alignement;
	size -= alignement;

	while (size > 63)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((float*)src + 4);
		tmp3 = _mm_loadu_ps((float*)src + 8);
		tmp4 = _mm_loadu_ps((float*)src + 12);
		_mm_stream_ps((float*)dest, tmp);
		_mm_stream_ps((float*)dest + 4, tmp2);
		_mm_stream_ps((float*)dest + 8, tmp3);
		_mm_stream_ps((float*)dest + 12, tmp4);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 31)
	{
		tmp = _mm_loadu_ps(src);
		tmp2 = _mm_loadu_ps((float*)src + 4);
		_mm_stream_ps((float*)dest, tmp);
		_mm_stream_ps((float*)dest + 4, tmp2);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 15)
	{
		tmp = _mm_loadu_ps(src);
		_mm_stream_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 7)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 3)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 1)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
	_mm_sfence();
}

AVX_TARGET void fast_memcpy_nt_avx(void* dest, void* src, size_t size)
{
	__m256 tmp, tmp2;
	__m128 tmp128;
	
	size_t alignement = 32 - ((size_t)dest % 32);

	fast_memcpy(dest, src, alignement);
	dest = (uint8_t*)dest + alignement;
	src = (uint8_t*)src + alignement;
	size -= alignement;

	while (size > 63)
	{
		tmp = _mm256_loadu_ps((float*)src);
		tmp2 = _mm256_loadu_ps((float*)src + 8);
		_mm256_stream_ps((float*)dest, tmp);
		_mm256_stream_ps((float*)dest + 8, tmp2);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 31)
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_stream_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 15)
	{
		tmp128 = _mm_loadu_ps(src);
		_mm_stream_ps((float*)dest, tmp128);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 7)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 3)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 1)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
	_mm_sfence();
}

#endif // x86