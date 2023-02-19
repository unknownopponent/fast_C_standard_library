
#include "memmove_x86.h"

#if defined(x86_64) || defined(x86_32)

SSE_TARGET void* fast_memmove_sse(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	void* dest = destination;
	__m128 a;
	__m128 b;

	if (destination < source)
	{
		if (size >= 2 * sizeof(__m128))
		{
			a = _mm_loadu_ps((float*)source);
			while (size >= 2 * sizeof(__m128))
			{
				b = _mm_loadu_ps((float*)(((__m128*)source) + 1));
				_mm_storeu_ps((float*)destination, a);
				a = b;
				source = (__m128*)source + 1;
				destination = (__m128*)destination + 1;
				size -= sizeof(__m128);
			}

			_mm_storeu_ps((float*)destination, a);
			source = (__m128*)source + 1;
			destination = (__m128*)destination + 1;
			size -= sizeof(__m128);
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source + 1;
			destination = (int8_t*)destination + 1;
			size -= sizeof(int8_t);
		}
	}
	else
	{
		if (size >= 2 * sizeof(__m128))
		{
			destination = (int8_t*)destination + size - sizeof(__m128);
			source = (int8_t*)source + size - sizeof(__m128);

			a = _mm_loadu_ps((float*)source);
			while (size > 2 * sizeof(__m128))
			{
				b = _mm_loadu_ps((float*)(((__m128*)source) - 1));
				_mm_storeu_ps((float*)destination, a);
				a = b;
				source = (__m128*)source - 1;
				destination = (__m128*)destination - 1;
				size -= sizeof(__m128);
			}

			_mm_storeu_ps((float*)destination, a);
			destination = (int8_t*)destination - sizeof(int8_t);
			source = (int8_t*)source - sizeof(int8_t);
			size -= sizeof(__m128);
		}
		else
		{
			destination = (int8_t*)destination + size - 1;
			source = (int8_t*)source + size - 1;
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source - 1;
			destination = (int8_t*)destination - 1;
			size -= sizeof(int8_t);
		}
	}
	return dest;
}

AVX_TARGET void* fast_memmove_avx(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	void* dest = destination;
	__m256 a;
	__m256 b;

	if (destination < source)
	{
		if (size >= 2 * sizeof(__m256))
		{
			a = _mm256_loadu_ps((float*)source);
			while (size >= 2 * sizeof(__m256))
			{
				b = _mm256_loadu_ps((float*)(((__m256*)source) + 1));
				_mm256_storeu_ps((float*)destination, a);
				a = b;
				source = (__m256*)source + 1;
				destination = (__m256*)destination + 1;
				size -= sizeof(__m256);
			}

			_mm256_storeu_ps((float*)destination, a);
			source = (__m256*)source + 1;
			destination = (__m256*)destination + 1;
			size -= sizeof(__m256);
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source + 1;
			destination = (int8_t*)destination + 1;
			size -= sizeof(int8_t);
		}
	}
	else
	{
		if (size >= 2 * sizeof(__m256))
		{
			destination = (int8_t*)destination + size - sizeof(__m256);
			source = (int8_t*)source + size - sizeof(__m256);

			a = _mm256_loadu_ps((float*)source);
			while (size > 2 * sizeof(__m256))
			{
				b = _mm256_loadu_ps((float*)(((__m256*)source) - 1));
				_mm256_storeu_ps((float*)destination, a);
				a = b;
				source = (__m256*)source - 1;
				destination = (__m256*)destination - 1;
				size -= sizeof(__m256);
			}

			_mm256_storeu_ps((float*)destination, a);
			destination = (int8_t*)destination - sizeof(int8_t);
			source = (int8_t*)source - sizeof(int8_t);
			size -= sizeof(__m256);
		}
		else
		{
			destination = (int8_t*)destination + size - 1;
			source = (int8_t*)source + size - 1;
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source - 1;
			destination = (int8_t*)destination - 1;
			size -= sizeof(int8_t);
		}
	}
	return dest;
}

SSE_TARGET void* fast_memmove_sse_nt(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	void* dest = destination;
	__m128 a;
	__m128 b;

	if (destination < source)
	{
		//align
		while ((size_t)destination %16)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source + 1;
			destination = (int8_t*)destination + 1;
			size -= sizeof(int8_t);
		}

		if (size >= 3 * sizeof(__m128))
		{
			a = _mm_loadu_ps((float*)source);
			while (size >= 2 * sizeof(__m128))
			{
				b = _mm_loadu_ps((float*)(((__m128*)source) + 1));
				_mm_stream_ps((float*)destination, a);
				a = b;
				source = (__m128*)source + 1;
				destination = (__m128*)destination + 1;
				size -= sizeof(__m128);
			}

			_mm_stream_ps((float*)destination, a);
			source = (__m128*)source + 1;
			destination = (__m128*)destination + 1;
			size -= sizeof(__m128);
		}

		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source + 1;
			destination = (int8_t*)destination + 1;
			size -= sizeof(int8_t);
		}
	}
	else
	{
		if (size >= 3 * sizeof(__m128))
		{
			destination = (int8_t*)destination + size - sizeof(int8_t);
			source = (int8_t*)source + size - sizeof(int8_t);

			while (((size_t)destination +1)% 16)
			{
				*(int8_t*)destination = *(int8_t*)source;
				source = (int8_t*)source - 1;
				destination = (int8_t*)destination - 1;
				size -= sizeof(int8_t);
			}

			destination = (int8_t*)destination + sizeof(int8_t) - sizeof(__m128);
			source = (int8_t*)source + sizeof(int8_t) - sizeof(__m128);

			a = _mm_loadu_ps((float*)source);
			while (size > 2 * sizeof(__m128))
			{
				b = _mm_loadu_ps((float*)(((__m128*)source) - 1));
				_mm_stream_ps((float*)destination, a);
				a = b;
				source = (__m128*)source - 1;
				destination = (__m128*)destination - 1;
				size -= sizeof(__m128);
			}

			_mm_stream_ps((float*)destination, a);
			destination = (int8_t*)destination - sizeof(int8_t);
			source = (int8_t*)source - sizeof(int8_t);
			size -= sizeof(__m128);
		}
		else
		{
			destination = (int8_t*)destination + size - 1;
			source = (int8_t*)source + size - 1;
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source - 1;
			destination = (int8_t*)destination - 1;
			size -= sizeof(int8_t);
		}
	}
	_mm_sfence();
	return dest;
}

AVX_TARGET void* fast_memmove_avx_nt(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	void* dest = destination;
	__m256 a;
	__m256 b;

	if (destination < source)
	{
		if (size >= 3 * sizeof(__m256))
		{
			//align
			while ((size_t)destination % 32)
			{
				*(int8_t*)destination = *(int8_t*)source;
				source = (int8_t*)source + 1;
				destination = (int8_t*)destination + 1;
				size -= sizeof(int8_t);
			}

			a = _mm256_loadu_ps((float*)source);
			while (size >= 2 * sizeof(__m256))
			{
				b = _mm256_loadu_ps((float*)(((__m256*)source) + 1));
				_mm256_stream_ps((float*)destination, a);
				a = b;
				source = (__m256*)source + 1;
				destination = (__m256*)destination + 1;
				size -= sizeof(__m256);
			}

			_mm256_stream_ps((float*)destination, a);
			source = (__m256*)source + 1;
			destination = (__m256*)destination + 1;
			size -= sizeof(__m256);
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source + 1;
			destination = (int8_t*)destination + 1;
			size -= sizeof(int8_t);
		}
	}
	else
	{
		if (size >= 3 * sizeof(__m256))
		{
			destination = (int8_t*)destination + size - sizeof(int8_t);
			source = (int8_t*)source + size - sizeof(int8_t);

			while (((size_t)destination + 1) % 32)
			{
				*(int8_t*)destination = *(int8_t*)source;
				source = (int8_t*)source - 1;
				destination = (int8_t*)destination - 1;
				size -= sizeof(int8_t);
			}

			destination = (int8_t*)destination + sizeof(int8_t) - sizeof(__m256);
			source = (int8_t*)source + sizeof(int8_t) - sizeof(__m256);

			a = _mm256_loadu_ps((float*)source);
			while (size > 2 * sizeof(__m256))
			{
				b = _mm256_loadu_ps((float*)(((__m256*)source) - 1));
				_mm256_stream_ps((float*)destination, a);
				a = b;
				source = (__m256*)source - 1;
				destination = (__m256*)destination - 1;
				size -= sizeof(__m256);
			}

			_mm256_stream_ps((float*)destination, a);
			destination = (int8_t*)destination - sizeof(int8_t);
			source = (int8_t*)source - sizeof(int8_t);
			size -= sizeof(__m256);
		}
		else
		{
			destination = (int8_t*)destination + size - 1;
			source = (int8_t*)source + size - 1;
		}
		while (size)
		{
			*(int8_t*)destination = *(int8_t*)source;
			source = (int8_t*)source - 1;
			destination = (int8_t*)destination - 1;
			size -= sizeof(int8_t);
		}
	}
	_mm_sfence();
	return dest;
}

#endif