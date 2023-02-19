
#pragma once

#ifdef _WIN32

#pragma comment(lib, "onecore.lib")

#include <windows.h>
#include <memoryapi.h>

static int init = 0;

char init_large_pages()
{
	HANDLE token = 0;

	BOOL ret = OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&token
	);

	if (!ret)
	{
		return 1;
	}

	TOKEN_PRIVILEGES privileges = { 0 };
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	ret = LookupPrivilegeValueA(
		0,
		"SeLockMemoryPrivilege",
		&privileges.Privileges[0].Luid
	);

	if (!ret)
	{
		return 1;
	}

	ret = AdjustTokenPrivileges(
		token,
		0,
		&privileges,
		0,
		0,
		0
	);

	if (!ret)
	{
		return 1;
	}

	init = 1;
	return 0;
}

void* alloc_large_page(size_t size)
{
	if (!init)
	{
		if (init_large_pages())
		{
			return 0;
		}
	}

	SIZE_T min = GetLargePageMinimum();
	if (!min)
	{
		return 0;
	}

	size_t new_size = (size / min +1)* min;

	return VirtualAlloc(
		0, 
		new_size,
		MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, 
		PAGE_READWRITE
	);
}

#endif