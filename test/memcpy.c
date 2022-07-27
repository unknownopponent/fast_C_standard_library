
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/Timer.h"

char bench(char* src, char* dst, size_t size, volatile void(*cpy)(void* dest, void* src, size_t size), size_t iterations)
{
	for (size_t i = 0; i < size; i++)
		src[i] = 85;
	for (size_t i = 0; i < size; i++)
		dst[i] = -42;

	Timer timer;

	start(&timer);
	for (size_t i = 0; i < iterations; i++)
		cpy(dst, src, size);
	uint64_t execution_time_ns = end(&timer);

	if (execution_time_ns >= 1000000000)
		printf("%llus ", execution_time_ns / 1000000000);
	if (execution_time_ns >= 1000000)
		printf("%llums ", (execution_time_ns / 1000000) % 1000);
	if (execution_time_ns >= 1000)
		printf("%lluus ", (execution_time_ns / 1000) % 1000);
	printf("%lluns\n", execution_time_ns % 1000);

	for (size_t i = 0; i < size; i++)
	{
		if (src[i] != dst[i])
		{
			fprintf(stderr, "source and destination are different at %llu, [%d] [%d]\n", i, src[i], dst[i]);
			return 1;
		}
	}
	return 0;
}

void dummy_cpy(void* dest, void* src, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
}

//force FAST_MEMCPY and FAST_MEMCPY_NT
//#define NO_ASM
#define USE_SSE 
#define USE_AVX

#include "../fast_C_standard_library/fast_memcpy.h"

char example()
{
	volatile char src[1024];
	volatile char dst[1024];
	
	for (size_t i = 0; i < 1024; i++)
		src[i] = 85;
	
	//memcpy for memory smaller than cache
	FAST_MEMCPY((void*)dst, (void*)src, 1024);
	
	for (size_t i = 0; i < 1024; i++)
	{
		if (src[i] != dst[i])
		{
			fprintf(stderr, "source and destination are different at %llu, [%d] [%d]\n", i, src[i], dst[i]);
			return 1;
		}
	}
	
	//memcpy for memory bigger than cache
	FAST_MEMCPY_NT((void*)dst, (void*)src, 1024);

	for (size_t i = 0; i < 1024; i++)
	{
		if (src[i] != dst[i])
		{
			fprintf(stderr, "source and destination are different at %llu, [%d] [%d]\n", i, src[i], dst[i]);
			return 1;
		}
	}

	puts("example passed\n");

	return 0;
}

char benchmarks()
{
	size_t size = 100000000; // depending cpu cache
	size_t iterations = 100;

	char* src = malloc(size);
	if (!src)
		return 1;
	char* dst = malloc(size);
	if (!dst)
	{
		free(src);
		return 1;
	}
	
	printf("large memory block copy %llu bytes %llu iterations\n", size, iterations);

	printf("dummy_cpy\t");
	if (bench(src, dst, size, dummy_cpy, iterations))
		goto fail;
#if defined(SSE) || defined (USE_SSE)
	printf("fast_memcpy_nt_sse\t");
	if (bench(src, dst, size, fast_memcpy_nt_sse, iterations))
		goto fail;
#endif
#if defined(AVX) || defined(USE_AVX)
	printf("fast_memcpy_nt_avx\t");
	if (bench(src, dst, size, fast_memcpy_nt_avx, iterations))
		goto fail;
#endif
	printf("memcpy\t");
	if (bench(src, dst, size, (void(*)(void*, void*, size_t))memcpy, iterations))
		goto fail;
	
	size = 100000; // depending cpu cache
	iterations = 100000;

	printf("\nsmall memory block copy %llu bytes %llu iterations\n", size, iterations);
	
	printf("dummy_cpy\t");
	if (bench(src, dst, size, dummy_cpy, iterations))
		goto fail;
	printf("fast_memcpy\t");
	if (bench(src, dst, size, fast_memcpy, iterations))
		goto fail;
#if defined(SSE) || defined(USE_SSE)
	printf("fast_memcpy_sse\t");
	if (bench(src, dst, size, fast_memcpy_sse, iterations))
		goto fail;
#endif
#if defined(AVX) || defined(USE_AVX)
	printf("fast_memcpy_avx\t");
	if (bench(src, dst, size, fast_memcpy_avx, iterations))
		goto fail;
#endif
	printf("memcpy\t");
	if (bench(src, dst, size, (void(*)(void*, void*, size_t))memcpy, iterations))
		goto fail;

	free(dst);
	free(src);

	return 0;

fail:

	free(dst);
	free(src);

	return 1;
}

int main(void)
{
	if (example())
	{
		fprintf(stderr, "fast memcpy example failled\n");
		return 1;
	}
	if (benchmarks())
	{
		fprintf(stderr, "fast memcpy benchmarks failled\n");
		return 1;
	}
	return 0;
}