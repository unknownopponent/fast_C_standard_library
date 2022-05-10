#include "fast_memcpy.h"

void fast_memcpy(void* dest, void* src, size_t size)
{
	while (size > 128)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		*((uint64_t*)dest + 1) = *((uint64_t*)src + 1);
		*((uint64_t*)dest + 2) = *((uint64_t*)src + 2);
		*((uint64_t*)dest + 3) = *((uint64_t*)src + 3);
		*((uint64_t*)dest + 4) = *((uint64_t*)src + 4);
		*((uint64_t*)dest + 5) = *((uint64_t*)src + 5);
		*((uint64_t*)dest + 6) = *((uint64_t*)src + 6);
		*((uint64_t*)dest + 7) = *((uint64_t*)src + 7);
		*((uint64_t*)dest + 8) = *((uint64_t*)src + 8);
		*((uint64_t*)dest + 9) = *((uint64_t*)src + 9);
		*((uint64_t*)dest + 10) = *((uint64_t*)src + 10);
		*((uint64_t*)dest + 11) = *((uint64_t*)src + 11);
		*((uint64_t*)dest + 12) = *((uint64_t*)src + 12);
		*((uint64_t*)dest + 13) = *((uint64_t*)src + 13);
		*((uint64_t*)dest + 14) = *((uint64_t*)src + 14);
		*((uint64_t*)dest + 15) = *((uint64_t*)src + 15);
		dest = (uint64_t*)dest + 16;
		src = (uint64_t*)src + 16;
		size -= 128;
	}
	while (size > 64)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		*((uint64_t*)dest + 1) = *((uint64_t*)src + 1);
		*((uint64_t*)dest + 2) = *((uint64_t*)src + 2);
		*((uint64_t*)dest + 3) = *((uint64_t*)src + 3);
		*((uint64_t*)dest + 4) = *((uint64_t*)src + 4);
		*((uint64_t*)dest + 5) = *((uint64_t*)src + 5);
		*((uint64_t*)dest + 6) = *((uint64_t*)src + 6);
		*((uint64_t*)dest + 7) = *((uint64_t*)src + 7);
		dest = (uint64_t*)dest + 8;
		src = (uint64_t*)src + 8;
		size -= 64;
	}
	while (size > 32)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		*((uint64_t*)dest + 1) = *((uint64_t*)src + 1);
		*((uint64_t*)dest + 2) = *((uint64_t*)src + 2);
		*((uint64_t*)dest + 3) = *((uint64_t*)src + 3);
		dest = (uint64_t*)dest + 4;
		src = (uint64_t*)src + 4;
		size -= 32;
	}
	while (size > 16)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		*((uint64_t*)dest + 1) = *((uint64_t*)src + 1);
		dest = (uint64_t*)dest + 2;
		src = (uint64_t*)src + 2;
		size -= 16;
	}
	while (size > 8)
	{
		*((uint64_t*)dest) = *((uint64_t*)src);
		dest = (uint64_t*)dest + 1;
		src = (uint64_t*)src + 1;
		size -= 8;
	}
	while (size > 4)
	{
		*((uint32_t*)dest) = *((uint32_t*)src);
		dest = (uint32_t*)dest + 1;
		src = (uint32_t*)src + 1;
		size -= 4;
	}
	while (size > 2)
	{
		*((uint16_t*)dest) = *((uint16_t*)src);
		dest = (uint16_t*)dest + 1;
		src = (uint16_t*)src + 1;
		size -= 2;
	}
	while (size > 1)
	{
		*((uint8_t*)dest) = *((uint8_t*)src);
		dest = (uint8_t*)dest + 1;
		src = (uint8_t*)src + 1;
		size -= 1;
	}
}