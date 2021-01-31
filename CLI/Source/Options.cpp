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

#include "Options.h"

#include <thread>

#include "PlatformDef.h"

static std::vector<std::string> s_ValidOptions =
{
	"-threads", "-t",
	"-loglevel", "-ll",
	"-log", "-l",
	"-tilesize", "-ts"
};
static std::vector<std::string> s_ValidSwitches =
{
	"-help", "-h",
	"-quiet", "-q",
};

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

FILE* FileOpen(const char* filename, const char* mode)
{
	FILE* file;
	fopen_s(&file, filename, mode);
	return file;
}

#else

#include <unistd.h>

FILE* FileOpen(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

#endif

void ShowHelp();

Options GenerateOptions(const std::vector<std::string>& options)
{
	Options output;

	std::filesystem::path logFile = "Log.txt";

	for (const auto& option : options)
	{
		size_t i = option.find('=');
		auto name = option.substr(0, i);
		bool isOption = std::find(s_ValidOptions.begin(), s_ValidOptions.end(), name) != s_ValidOptions.end();
		bool isSwitch = std::find(s_ValidSwitches.begin(), s_ValidSwitches.end(), name) != s_ValidSwitches.end();

		if (!isOption && !isSwitch) { fmt::print(fg(fmt::color::yellow), "Ignoring unknown option '{}'.", option.c_str()); }

		if (isOption)
		{
			auto value = option.substr(i + 1);
			if (value.empty()) { fmt::print(fg(fmt::color::red), "Value of option '{}' cannot be empty.", name.c_str()); }

			if (name == "-threads" || name == "-t")
			{
				try
				{
					int count = stoi(value);
					if (count <= 0)
					{
						fmt::print(fg(fmt::color::red), "Thread count cannot be 0 or negative. Auto-detecting.");
						count = 0;
					}
					output.ThreadCount = count;
					if (std::thread::hardware_concurrency() > 0 && output.ThreadCount > std::thread::hardware_concurrency() * 2)
					{
						fmt::print(fg(fmt::color::yellow), "Thread count is much greater than the maximum concurrent threads supported ({}), you might lose some performance.",
							std::thread::hardware_concurrency());
					}
				}
				catch (std::exception) { fmt::print(fg(fmt::color::red), "Thread count must be an integer. Auto-detecting."); }
			}
			else if (name == "-tilesize" || name == "-ts")
			{
				try
				{
					int count = stoi(value);
					if (count <= 0)
					{
						fmt::print(fg(fmt::color::red), "Tile size cannot be 0 or negative. Using default ({}).", output.TileSize);
						count = 0;
					}
					output.TileSize = count;
				}
				catch (std::exception) { fmt::print(fg(fmt::color::red), "Tile size must be an integer. Using default ({}).", output.TileSize); }
			}
			else if (name == "-loglevel" || name == "-ll")
			{
				try
				{
					int level = stoi(value);
					if (level < 0 || level > 4)
					{
						fmt::print(fg(fmt::color::red), "Log level must be between 0 and 4 (inclusive).");
						level = LL_Warning;
					}
					output.LoggingLevel = LogLevel(level);
				}
				catch (std::exception) { fmt::print(fg(fmt::color::red), "Log level must be an integer."); }
			}
			else if (name == "-log" || name == "-l")
			{
				logFile = value;
				if (!logFile.has_filename())
				{
					fmt::print(fg(fmt::color::dark_red), "Log file must be a file, not directory!");
					throw -1;
				}

				try { std::filesystem::create_directories(logFile.root_path()); }
				catch (...)
				{
					fmt::print(fg(fmt::color::dark_red), "Could not create log file '{}'", value);
					throw -1;
				}
			}
		}
		else
		{
			if (option == "-help" || option == "-h")
			{
				ShowHelp();
				exit(0);
			}
			else if (option == "-quiet" || option == "-q")
			{
				output.Quiet = true;
				output.LoggingLevel = LL_Fatal;
			}
		}
	}

	output.LogFile = FileOpen(logFile.string().c_str(), "w");
	if (!output.LogFile)
	{
		fmt::print(fg(fmt::color::red), "Could not open log file. File logging is disabled.");
	}

	return output;
}

void ShowHelp()
{
	fmt::print(fg(fmt::color::dark_orange), R"(Cinder is a modular and easy to extend raytracer.

Usage: Cinder [option/filename] [option/filename]...
Options: 
  -help/-h               What you are looking at right now.
  -threads/-t=<n>        Number of threads to use for rendering.
                         Do not specify to auto-detect from the number of cores available.
  -nologo                Suppress the banner on startup.
  -quiet/-q              Suppress everything except fatal errors. Implies -nologo.
  -log/-l                Location of the log file. If left blank, the file will be called
                         'Log.txt' in the current working directory.
  -loglevel/-ll=<n>      <n> must be from 0 - 4:
                         0 - Verbose
                         1 - Log
                         2 - Warning
                         3 - Error
                         4 - Fatal
  -tilesize/-ts=<n>      <n> is the length of each tile in a sampler-based renderer.
                         The default is 16
)");
}
