#include "PCH.h"

int Entry(int argc, char** argv);

#ifndef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	return Entry(argc, argv);
}

#else
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

std::string ToUTF8(const wchar_t* utf16)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, nullptr, 0, nullptr, nullptr);
	std::string temp(size, 0);

	WideCharToMultiByte(CP_UTF8, 0, utf16, -1, temp.data(), size, nullptr, nullptr);
	temp.pop_back(); // Fixes a weird issue where we have double nulls

	return temp;
}

int wmain(int argc, wchar_t** argv)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	if (output == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Error: Failed to get console handle! Something is really wrong here. \n");
		return EXIT_FAILURE;
	}

	DWORD outMode;
	if (!GetConsoleMode(output, &outMode))
	{
		wprintf(L"Error: Could not get console mode! This should not be happening. \n");
		return EXIT_FAILURE;
	}

	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(output, outMode))
	{
		wprintf(L"Error: Failed to set console output mode! That was unexpected. \n");
		return EXIT_FAILURE;
	}

	auto uArgv = new char* [argc];
	auto strings = new std::string[argc];
	for (int i = 0; i < argc; i++)
	{
		strings[i] = ToUTF8(argv[i]);
		uArgv[i] = strings[i].data();
	}

	return Entry(argc, uArgv);
}

#endif
