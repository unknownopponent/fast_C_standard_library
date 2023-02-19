#pragma once

#include <stdint.h>
#include <string.h>

#include "common.h"

void dummy_memcpy(void* restrict dest, void* restrict src, size_t size);

void fast_memcpy(void* restrict dest, void* restrict src, size_t size);