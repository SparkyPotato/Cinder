#pragma once

#ifdef PLATFORM_WINDOWS

std::string ToUTF8(const wchar_t* utf16);

#define BREAK_IF(x) if (x) { __debugbreak(); }

#else

#define BREAK_IF(x) 

#endif

FILE* FileOpen(const char* filename, const char* mode);
