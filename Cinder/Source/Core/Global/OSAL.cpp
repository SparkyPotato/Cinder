#include "PCH.h"

#include "OSAL.h"

#ifdef PLATFORM_WINDOWS

FILE* FileOpen(const char* filename, const char* mode)
{
	FILE* file;
	fopen_s(&file, filename, mode);
	return file;
}

#else

FILE* FileOpen(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

#endif
