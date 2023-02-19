
#include "memcmp.h"

#include <stdint.h>

int dummy_memcmp(void* ptr1, void* ptr2, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		int test = (int)((uint8_t*)ptr1)[i] - (int)((uint8_t*)ptr2)[i];
		if (test)
		{
			return test;
		}
	}
	return 0;
}