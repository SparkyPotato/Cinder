#pragma once

#ifdef PLATFORM_WINDOWS

std::string ToUTF8(const wchar_t* utf16);

#endif

FILE* FileOpen(const char* filename, const char* mode);
