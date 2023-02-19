
#include "memmove.h"

#include <stdint.h>

void* dummy_memmove(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	if (destination < source)
	{
		for (size_t i = 0; i < size; i++)
		{
			((uint8_t*)destination)[i] = ((uint8_t*)source)[i];
		}
	}
	else
	{
		for (int64_t i = size -1; i >= 0; i--)
		{
			((uint8_t*)destination)[i] = ((uint8_t*)source)[i];
		}
	}
	
	return destination;
}

void* fast_memmove(void* destination, void* source, size_t size)
{
	if (destination == source)
	{
		return destination;
	}

	void* dest = destination;
	int64_t a;
	int64_t b;

	if (destination < source)
	{
		if (size >= 2*sizeof(int64_t))
		{
			a = *(int64_t*)source;
			while (size >= 2 * sizeof(int64_t))
			{
				b = *((int64_t*)(source)+1);
				*(int64_t*)destination = a;
				a = b;
				source = (int64_t*)source + 1;
				destination = (int64_t*)destination + 1;
				size -= sizeof(int64_t);
			}
			*(int64_t*)destination = a;
			source = (int64_t*)source + 1;
			destination = (int64_t*)destination + 1;
			size -= sizeof(int64_t);
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
		if (size >= 2 * sizeof(int64_t))
		{
			destination = (int8_t*)destination + size - sizeof(int64_t);
			source = (int8_t*)source + size - sizeof(int64_t);
		
			a = *(int64_t*)source;
			while (size > 2 * sizeof(int64_t))
			{
				b = *((int64_t*)source-1);
				*(int64_t*)destination = a;
				a = b;
				source = (int64_t*)source - 1;
				destination = (int64_t*)destination - 1;
				size -= sizeof(int64_t);
			}

			*(int64_t*)destination = a;
			destination = (int8_t*)destination - sizeof(int8_t);
			source = (int8_t*)source - sizeof(int8_t);
			size -= sizeof(int64_t);
		}
		else
		{
			destination = (int8_t*)destination + size -1;
			source = (int8_t*)source + size -1;
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