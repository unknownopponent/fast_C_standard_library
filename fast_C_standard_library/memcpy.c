#include "memcpy.h"

void dummy_memcpy(void* restrict dest, void* restrict src, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
}

void fast_memcpy(void* restrict dest, void* restrict src, size_t size)
{
	while (size > 7)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 0)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}