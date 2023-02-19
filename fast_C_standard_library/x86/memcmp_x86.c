
#include "memcmp_x86.h"

#if defined(x86_32) || defined(x86_64)

#include <stdint.h>

SSE2_TARGET int memcmp_sse2(void* ptr1, void* ptr2, size_t num)
{
	__m128i xmm0, xmm1;

	size_t i = 0;

	while (i < num - num% sizeof(__m128i))
	{
		xmm0 = _mm_loadu_si128((__m128i*)ptr1);
		xmm1 = _mm_loadu_si128((__m128i*)ptr2);

		xmm0 = _mm_cmpeq_epi8(xmm0, xmm1);

		uint16_t mask = _mm_movemask_epi8(xmm0);
		mask = ~mask;

		if (mask)
		{
			int trailing_zeros = _tzcnt_u32(mask);

			return (int)((uint8_t*)ptr1)[trailing_zeros] - (int)((uint8_t*)ptr2)[trailing_zeros];
		}

		ptr1 = ((__m128i*)ptr1) + 1;
		ptr2 = ((__m128i*)ptr2) + 1;
		i += sizeof(__m128i);
	}

	while (i < num)
	{
		int test = (int)*(uint8_t*)ptr1 - (int)*(uint8_t*)ptr2;
		if (test)
		{
			return test;
		}
	}

	return 0;
}

AVX2_TARGET int memcmp_avx2(void* ptr1, void* ptr2, size_t num)
{
	__m256i ymm0, ymm1;

	size_t i = 0;

	while (i < num - num % sizeof(__m256i))
	{
		ymm0 = _mm256_loadu_si256((__m256i*)ptr1);
		ymm1 = _mm256_loadu_si256((__m256i*)ptr2);

		ymm0 = _mm256_cmpeq_epi8(ymm0, ymm1);

		uint32_t mask = _mm256_movemask_epi8(ymm0);
		mask = ~mask;

		if (mask)
		{
			int trailing_zeros = _tzcnt_u32(mask);

			return (int)((uint8_t*)ptr1)[trailing_zeros] - (int)((uint8_t*)ptr2)[trailing_zeros];
		}

		ptr1 = ((__m256i*)ptr1) + 1;
		ptr2 = ((__m256i*)ptr2) + 1;
		i += sizeof(__m256i);
	}

	while (i < num)
	{
		int test = (int)*(uint8_t*)ptr1 - (int)*(uint8_t*)ptr2;
		if (test)
		{
			return test;
		}
	}

	return 0;
}

#endif