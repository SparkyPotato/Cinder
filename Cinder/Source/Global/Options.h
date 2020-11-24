#pragma once

// Command line options for Cinder
struct Options
{
	uint16_t ThreadCount = 0;
};

Options GenerateOptions(const std::vector<std::string>& options);
