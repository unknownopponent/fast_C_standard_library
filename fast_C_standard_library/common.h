
#pragma once

#ifdef  __GNUC__
#define restrict __restrict__
#elif defined(_MSC_VER)
#define restrict __restrict
#endif