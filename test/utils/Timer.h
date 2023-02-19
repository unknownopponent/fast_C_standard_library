
#pragma once

#include <stdint.h>

typedef struct Timer Timer;

void start(Timer* timer);
uint64_t end(Timer* timer);


#ifdef _WIN32
#include <windows.h>

struct Timer
{
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;
};

void start(Timer* timer)
{
	QueryPerformanceFrequency(&timer->Frequency);
	QueryPerformanceCounter(&timer->StartingTime);
}

uint64_t end(Timer* timer)
{
	LARGE_INTEGER EndingTime;
	QueryPerformanceCounter(&EndingTime);

	LARGE_INTEGER ElapsedMicroseconds;
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - timer->StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000000;
	ElapsedMicroseconds.QuadPart /= timer->Frequency.QuadPart;

	return ElapsedMicroseconds.QuadPart;
}
#endif

#ifdef __linux__

#include <time.h>

struct Timer {
	struct timespec time1;
};

void start(Timer* timer)
{
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer->time1);
}

uint64_t end(Timer* timer)
{
	struct timespec time2;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	return (time2.tv_sec - timer->time1.tv_sec) * 1000000000 + time2.tv_nsec - timer->time1.tv_nsec;
}

#endif

void print_nano(uint64_t nano)
{
	if (nano >= 1000000000)
		printf("%llus ", nano / 1000000000);
	if (nano >= 1000000)
		printf("%llums ", (nano / 1000000) % 1000);
	if (nano >= 1000)
		printf("%lluus ", (nano / 1000) % 1000);
	printf("%lluns", nano % 1000);
}