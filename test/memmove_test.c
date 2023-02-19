
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memmove.h"
#include "x86/memmove_x86.h"
#include "utils/Timer.h"
#include "utils/large_pages.h"

#define TEST_SIZE 1024

char test()
{
	volatile void* (*memmove_funct[])(void*, void*, size_t) = {
		dummy_memmove,
		fast_memmove,
		fast_memmove_sse,
		fast_memmove_avx,
		fast_memmove_sse_nt,
		fast_memmove_avx_nt,
	};

	char* names[] = {
		"dummy_memmove",
		"fast_memmove",
		"fast_memmove_sse",
		"fast_memmove_avx",
		"fast_memmove_sse_nt",
		"fast_memmove_avx_nt",
	};

	char block[TEST_SIZE];
	char control[TEST_SIZE];

	for (size_t i = 0; i < TEST_SIZE / 2; i++)
	{
		for (size_t j = 0; j < TEST_SIZE; j++)
		{
			control[j] = j;
		}

		memmove(control + TEST_SIZE / 4 +3, control + i, TEST_SIZE / 2);

		for (size_t j = 0; j < sizeof(memmove_funct) / sizeof(void*); j++)
		{
			for (size_t k = 0; k < TEST_SIZE; k++)
			{
				block[k] = k;
			}

			void* ret = memmove_funct[j](block + TEST_SIZE / 4 +3, block + i, TEST_SIZE / 2);

			if (ret != block + TEST_SIZE / 4 +3)
			{
				printf("%s return wrong pointer\n", names[j]);
				return 1;
			}
			if (memcmp(block, control, TEST_SIZE))
			{
				printf("%s gives wrong result\n", names[j]);
				return 1;
			}
		}
	}

	puts("tests passed\n");

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
	char* src = ALLOC(LARGE_SIZE);
	if (!src)
	{
		printf("failled to alloc %llu bytes", (uint64_t)LARGE_SIZE);
		return 1;
	}

	volatile void* (*memmove_funct[])(void*, void*, size_t) = {
		dummy_memmove,
		fast_memmove,
		fast_memmove_sse,
		fast_memmove_avx,
		memmove
	};

	char* names[] = {
		"dummy_memmove",
		"fast_memmove",
		"fast_memmove_sse",
		"fast_memmove_avx",
		"memmove"
	};

	printf("small memory block overlapped move %llu bytes %llu iterations\n", 
		(size_t)SMALL_SIZE, 
		(size_t)SMALL_ITERATIONS
	);

	for (size_t j = 0; j < sizeof(memmove_funct) / sizeof(void*); j++)
	{
		printf("%s\t", names[j]);

		Timer timer;

		start(&timer);
		for (size_t i = 0; i < SMALL_ITERATIONS; i++)
		{
			memmove_funct[j](src + 1, src, SMALL_SIZE - 1);
		}
		uint64_t execution_time_ns = end(&timer);

		print_nano(execution_time_ns);
		printf("\n");
	}


	volatile void* (*memmove_nt_funct[])(void*, void*, size_t) = {
		dummy_memmove,
		fast_memmove,
		fast_memmove_sse,
		fast_memmove_avx,
		fast_memmove_sse_nt,
		fast_memmove_avx_nt,
		memmove
	};

	char* nt_names[] = {
		"dummy_memmove",
		"fast_memmove",
		"fast_memmove_sse",
		"fast_memmove_avx",
		"fast_memmove_sse_nt",
		"fast_memmove_avx_nt",
		"memmove"
	};

	printf("\nlarge memory block overlapped move %llu bytes %llu iterations\n",
		(size_t)LARGE_SIZE,
		(size_t)LARGE_ITERATIONS
	);

	for (size_t j = 0; j < sizeof(memmove_nt_funct) / sizeof(void*); j++)
	{
		printf("%s\t", nt_names[j]);

		Timer timer;

		start(&timer);
		for (size_t i = 0; i < LARGE_ITERATIONS; i++)
		{
			memmove_nt_funct[j](src + 1, src, LARGE_SIZE - 1);
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
		fprintf(stderr, "memmove test failled\n");
		return 1;
	}
	if (benchmarks())
	{
		fprintf(stderr, "memmove benchmarks failled\n");
		return 1;
	}
	return 0;
}