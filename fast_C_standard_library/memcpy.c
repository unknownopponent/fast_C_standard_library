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
	while (size >= sizeof(int64_t))
	{
		*((int64_t*)dest) = *((int64_t*)src);
		dest = (int64_t*)dest + 1;
		src = (int64_t*)src + 1;
		size -= sizeof(int64_t);
	}
	while (size)
	{
		*((int8_t*)dest) = *((int8_t*)src);
		dest = (int8_t*)dest + 1;
		src = (int8_t*)src + 1;
		size -= sizeof(int8_t);
	}
}