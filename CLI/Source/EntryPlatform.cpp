//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PlatformDef.h"

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

#include <cstdio>
#include <string>

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

	// while (!IsDebuggerPresent()) { Sleep(100); }

	return Entry(argc, uArgv);
}

#endif
