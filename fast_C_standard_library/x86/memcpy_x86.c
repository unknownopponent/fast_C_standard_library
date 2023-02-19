
#include "memcpy_x86.h"
#include "../memcpy.h"

#if defined(x86_64) || defined(x86_32)

SSE_TARGET void fast_memcpy_sse(void* restrict dest, void* restrict src, size_t size)
{
	__m128 tmp;
	
	while (size >= sizeof(__m128))
	{
		tmp = _mm_loadu_ps((float*)src);
		_mm_storeu_ps((float*)dest, tmp);
		dest = (__m128*)dest + 1;
		src = (__m128*)src + 1;
		size -= sizeof(__m128);
	}
	while (size)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}
}

AVX_TARGET void fast_memcpy_avx(void* restrict dest, void* restrict src, size_t size)
{
	__m256 tmp;
	
	while (size >= sizeof(__m256))
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_storeu_ps((float*)dest, tmp);
		dest = (__m256*)dest + 1;
		src = (__m256*)src + 1;
		size -= sizeof(__m256);
	}
	while (size)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}
}

SSE_TARGET void fast_memcpy_nt_sse(void* restrict dest, void* restrict src, size_t size)
{
	__m128 tmp;
	
	//align
	while ((size_t)dest % 16)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= 1;
	}

	while (size >= sizeof(__m128))
	{
		tmp = _mm_loadu_ps(src);
		_mm_stream_ps((float*)dest, tmp);
		dest = (__m128*)dest + 1;
		src = (__m128*)src + 1;
		size -= sizeof(__m128);
	}
	while (size)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}
	_mm_sfence();
}

AVX_TARGET void fast_memcpy_nt_avx(void* restrict dest, void* restrict src, size_t size)
{
	__m256 tmp;
	
	//align
	while ((size_t)dest % 32)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}

	while (size >= sizeof(__m256))
	{
		tmp = _mm256_loadu_ps((float*)src);
		_mm256_stream_ps((float*)dest, tmp);
		dest = (__m256*)dest + 1;
		src = (__m256*)src + 1;
		size -= sizeof(__m256);
	}
	while (size)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}
	_mm_sfence();
}

#endif // x86