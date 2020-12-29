#pragma once

#include "fmt/os.h"

// Command line options for Cinder
struct Options
{
	uint16_t ThreadCount = 0;
	uint32_t TileSize = 16;
};

namespace LogLevel
{
	constexpr int Verbose = 0;
	constexpr int Log = 1;
	constexpr int Warning = 2;
	constexpr int Error = 3;
	constexpr int Fatal = 4;
};

extern int GLogLevel;
extern bool GQuiet;
extern FILE* GLogFile;
extern Options GOptions;

Options GenerateOptions(const std::vector<std::string>& options);
