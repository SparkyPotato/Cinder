#include "PCH.h"
#include "Core/Global/OSAL.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

FILE* FileOpen(const char* filename, const char* mode)
{
	FILE* file;
	fopen_s(&file, filename, mode);
	return file;
}

#else

#include <unistd.h>

FILE* FileOpen(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

#endif
