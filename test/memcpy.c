
//#define NO_ASM
//#define SSE // same as compiler instruction set flags

#include "../fast_C_standard_library/fast_memcpy.h"

#include <stdlib.h>
#define SIZE 1023

int main()
{
	char* src = malloc(SIZE);
	if (!src)
		return 1;
	for (size_t i = 0; i < SIZE; i++)
		src[i] = i;

	char* dst = malloc(SIZE);
	if (!dst)
		return 1;

	for (size_t i = 0; i < 5000; i++)
		FAST_MEMCPY(dst, src, SIZE);

	return 0;
}
