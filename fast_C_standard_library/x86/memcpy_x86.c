
#include "memcpy_x86.h"
#include "../memcpy.h"

#if defined(x86_64) || defined(x86_32)

SSE_TARGET void fast_memcpy_sse(void* restrict dest, void* restrict src, size_t size)
{
	__m128 tmp;
	
	while (size > 15)
	{
		tmp = _mm_loadu_ps((float*)src);
		_mm_storeu_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

AVX_TARGET void fast_memcpy_avx(void* restrict dest, void* restrict src, size_t size)
{
	__m256 tmp;
	
	while (size > 31)
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_storeu_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}

SSE_TARGET void fast_memcpy_nt_sse(void* restrict dest, void* restrict src, size_t size)
{
	__m128 tmp;
	
	size_t alignement = 16 - ((size_t)dest % 16);

	fast_memcpy(dest, src, alignement);
	dest = (uint8_t*)dest + alignement;
	src = (uint8_t*)src + alignement;
	size -= alignement;

	while (size > 15)
	{
		tmp = _mm_loadu_ps(src);
		_mm_stream_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
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

AVX_TARGET void fast_memcpy_nt_avx(void* restrict dest, void* restrict src, size_t size)
{
	__m256 tmp;
	
	size_t alignement = 32 - ((size_t)dest % 32);

	fast_memcpy(dest, src, alignement);
	dest = (uint8_t*)dest + alignement;
	src = (uint8_t*)src + alignement;
	size -= alignement;

	while (size > 31)
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_stream_ps((float*)dest, tmp);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
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