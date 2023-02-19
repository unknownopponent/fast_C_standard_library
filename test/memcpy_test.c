
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/Timer.h"
#include "utils/large_pages.h"

#include "memcpy.h"
#include "x86/memcpy_x86.h"

#define TEST_SIZE 1024

char test()
{
	void* dest = malloc(TEST_SIZE);
	if (!dest)
	{
		return 1;
	}
	void* src = malloc(TEST_SIZE);
	if (!src)
	{
		return 1;
	}
	void* control = malloc(TEST_SIZE);
	if (!control)
	{
		return 1;
	}

	volatile void (*memcpy_funct[])(void*, void*, size_t) = {
		dummy_memcpy,
		fast_memcpy,
		fast_memcpy_sse,
		fast_memcpy_avx,
		fast_memcpy_nt_sse,
		fast_memcpy_nt_avx
	};

	char* names[] = {
		"dummy_memcpy",
		"fast_memcpy",
		"fast_memcpy_sse",
		"fast_memcpy_avx",
		"fast_memcpy_nt_sse",
		"fast_memcpy_nt_avx"
	};

	for (size_t i = 0; i < sizeof(memcpy_funct) / sizeof(void*); i++)
	{
		memset(src, rand(), TEST_SIZE);
		memcpy(control, src, TEST_SIZE);
		memcpy_funct[i](dest, src, TEST_SIZE);
		if (memcmp(control, dest, TEST_SIZE))
		{
			printf("%s gives different result\n", names[i]);
			return 1;
		}
	}

	puts("example passed\n");

	return 0;
}

#define SMALL_ITERATIONS 1000000
#define LARGE_ITERATIONS 200
// depending cpu cache
#define SMALL_SIZE 100000
#define LARGE_SIZE 100000000

//#define ALLOC alloc_large_page
#define ALLOC malloc

char benchmarks()
{
	void* dest = ALLOC(LARGE_SIZE);
	if (!dest)
	{
		fprintf(stderr, "failled to allocated %llu bytes\n", (uint64_t)LARGE_SIZE);
		return 1;
	}
	void* src = ALLOC(LARGE_SIZE);
	if (!src)
	{
		fprintf(stderr, "failled to allocated %llu bytes\n", (uint64_t)LARGE_SIZE);
		return 1;
	}

	volatile void* (*memcpy_funct[])(void*, void*, size_t) = {
		dummy_memcpy,
		fast_memcpy,
		fast_memcpy_sse,
		fast_memcpy_avx,
		memcpy
	};

	char* names[] = {
		"dummy_memcpy",
		"fast_memcpy",
		"fast_memcpy_sse",
		"fast_memcpy_avx",
		"memcpy"
	};

	printf("small memory block move %llu bytes %llu iterations\n",
		(size_t)SMALL_SIZE,
		(size_t)SMALL_ITERATIONS
	);

	for (size_t j = 0; j < sizeof(memcpy_funct) / sizeof(void*); j++)
	{
		printf("%s\t", names[j]);

		Timer timer;

		start(&timer);
		for (size_t i = 0; i < SMALL_ITERATIONS; i++)
		{
			memcpy_funct[j](dest, src, SMALL_SIZE);
		}
		uint64_t execution_time_ns = end(&timer);

		print_nano(execution_time_ns);
		printf("\n");
	}

	volatile void* (*memcpy_nt_funct[])(void*, void*, size_t) = {
		dummy_memcpy,
		fast_memcpy_nt_sse,
		fast_memcpy_nt_avx,
		memcpy
	};

	char* nt_names[] = {
		"dummy_memcpy",
		"fast_memcpy_nt_sse",
		"fast_memcpy_nt_avx",
		"memcpy"
	};

	printf("\nlarge memory block move %llu bytes %llu iterations\n",
		(size_t)LARGE_SIZE,
		(size_t)LARGE_ITERATIONS
	);

	for (size_t j = 0; j < sizeof(memcpy_nt_funct) / sizeof(void*); j++)
	{
		printf("%s\t", nt_names[j]);

		Timer timer;

		start(&timer);
		for (size_t i = 0; i < SMALL_ITERATIONS; i++)
		{
			memcpy_nt_funct[j](dest, src, SMALL_SIZE);
		}
		uint64_t execution_time_ns = end(&timer);

		print_nano(execution_time_ns);
		printf("\n");
	}

	return 0;
}

int main(void)
{
	if (test())
	{
		fprintf(stderr, "memcpy example failled\n");
		return 1;
	}
	if (benchmarks())
	{
		fprintf(stderr, "memcpy benchmarks failled\n");
		return 1;
	}
	return 0;
}