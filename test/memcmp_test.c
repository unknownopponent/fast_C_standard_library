
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "memcmp.h"
#include "x86/memcmp_x86.h"
#include "utils/Timer.h"

#define TEST_SIZE 1024

char test()
{
	srand(clock());

	void* one = malloc(TEST_SIZE);
	if (!one)
	{
		return 1;
	}
	void* two = malloc(TEST_SIZE);
	if (!two)
	{
		return 1;
	}

	volatile int (*memcmp_funct[])(void*, void*, size_t) = {
		dummy_memcmp,
		memcmp_sse2,
		memcmp_avx2,
	};

	char* names[] = {
		"dummy_memcmp",
		"memcmp_sse2",
		"memcmp_avx2",
	};

	for (size_t i = 0; i < TEST_SIZE; i++)
	{
		memcpy(two, one, TEST_SIZE);
		((char*)one)[i] = rand();

		int res = memcmp(one, two, TEST_SIZE);

		for (size_t j = 0; j < sizeof(memcmp_funct) / sizeof(void*); j++)
		{
			int tmp = memcmp_funct[j](one, two, TEST_SIZE);
			if (res == 0 && tmp !=0 || res < 0 && tmp >= 0 || res > 0 && tmp <= 0)
			{
				printf("%s returned different result\n", names[j]);
				return 1;
			}
		}
	}

	puts("tests passed");

	return 0;
}

#define SIZE 100000
#define ITERATIONS 1000000

char benchmarks()
{
	void* one = malloc(SIZE);
	if (!one)
	{
		return 1;
	}
	void* two = malloc(SIZE);
	if (!two)
	{
		return 1;
	}

	for (size_t i = 0; i < SIZE; i++)
	{
		((uint8_t*)one)[i] = 0xf0;
		((uint8_t*)two)[i] = 0xf0;
	}

	volatile int (*memcmp_funct[])(void*, void*, size_t) = {
		dummy_memcmp,
		memcmp_sse2,
		memcmp_avx2,
		memcmp
	};

	char* names[] = {
		"dummy_memcmp",
		"memcmp_sse2",
		"memcmp_avx2",
		"memcmp"
	};

	printf("memcmp %llu bytes %llu iterations\n",
		(size_t)SIZE,
		(size_t)ITERATIONS
	);

	for (size_t j = 0; j < sizeof(memcmp_funct) / sizeof(void*); j++)
	{
		printf("%s\t", names[j]);

		Timer timer;

		start(&timer);
		for (size_t i = 0; i < ITERATIONS; i++)
		{
			memcmp_funct[j](one, two, SIZE);
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
		fprintf(stderr, "memcmp test failled\n");
		return 1;
	}
	if (benchmarks())
	{
		fprintf(stderr, "memcmp benchmarks failled\n");
		return 1;
	}
	return 0;
}