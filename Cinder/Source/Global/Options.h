#pragma once

// Command line options for Cinder
struct Options
{
	uint16_t ThreadCount = 0;
};

namespace LogLevel
{
	inline int Verbose = 0;
	inline int Log = 1;
	inline int Warning = 2;
	inline int Error = 3;
	inline int Fatal = 4;
};

extern int GLogLevel;
extern bool GQuiet;

Options GenerateOptions(const std::vector<std::string>& options);
