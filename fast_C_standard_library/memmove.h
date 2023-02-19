
#pragma once

#include <stddef.h>

void* dummy_memmove(void* destination, void* source, size_t size);

void* fast_memmove(void* destination, void* source, size_t size);