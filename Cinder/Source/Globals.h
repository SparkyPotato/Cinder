#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace CommandLine
{
	std::map < std::wstring, std::wstring> Properties;
	std::vector<std::wstring> Switches;
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

struct Pixel
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};
