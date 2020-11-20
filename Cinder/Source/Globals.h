#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace CommandLine
{
	extern std::map <std::wstring, std::wstring> Properties;
	extern std::vector<std::wstring> Switches;
}

template<typename T>
void Output(const T& arg)
{
	std::wcout << arg << L"\n";
}

template<typename T, typename... Args>
void Output(const T& arg, Args&&... args)
{
	std::wcout << arg;

	Output(std::forward<Args>(args)...);
}

template<typename... Args>
void Error(Args&&... args)
{
	Output("Error: ", std::forward<Args>(args)...);

	throw -1;
}

template<typename... Args>
void Warning(Args&&... args)
{
	Output("Warning: ", std::forward<Args>(args)...);
}

void ParseCommandLine(int argc, wchar_t** argv);

struct Pixel
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct Framebuffer
{
	Framebuffer(uint64_t width, uint64_t height);

	inline Pixel& GetPixel(uint64_t x, uint64_t y) { return Buffer[x + y * Width]; }

	Pixel* Buffer;
	uint64_t Width;
	uint64_t Height;
};
