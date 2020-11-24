#include "PCH.h"
#include "Options.h"

void ShowHelp();

int GLogLevel = LogLevel::Warning;
bool GQuiet = false;

static std::vector<std::string> s_ValidOptions =
{
	"-threads", "-t",
	"-loglevel", "-l"
};
static std::vector<std::string> s_ValidSwitches =
{
	"-help", "-h",
	"-quiet", "-q",
};

Options GenerateOptions(const std::vector<std::string>& options)
{
	Options output;

	for (const auto& option : options)
	{
		size_t i = option.find('=');
		auto name = option.substr(0, i);
		bool isOption = std::find(s_ValidOptions.begin(), s_ValidOptions.end(), name) != s_ValidOptions.end();
		bool isSwitch = std::find(s_ValidSwitches.begin(), s_ValidSwitches.end(), name) != s_ValidSwitches.end();

		if (!isOption && !isSwitch) { Warning("Ignoring unknown option '%s'.", option.c_str()); }

		if (isOption)
		{
			auto value = option.substr(i + 1);
			if (value.empty()) { Error("Value of option '%s' cannot be empty.", name.c_str()); }
			
			if (name == "-threads" || name == "-t")
			{
				try
				{
					int count = stoi(value);
					if (count <= 0) 
					{ 
						Error("Thread count cannot be 0 or negative. Auto-detecting."); 
						count = 0;
					}
					output.ThreadCount = count;
					if (std::thread::hardware_concurrency() > 0 && output.ThreadCount > std::thread::hardware_concurrency() * 2)
					{
						Warning("%s is much greater than the maximum concurrent threads supported, you might lose some performance.", name.c_str());
					}
				}
				catch (std::exception) { Error("Thread count must be an integer. Auto-detecting."); }
			}
			else if (name == "-loglevel" || name == "-l")
			{
				try
				{
					int level = stoi(value);
					if (level < 0 || level > 4)
					{
						Error("Log level must be between 0 and 4 (inclusive).", name.c_str());
						level = LogLevel::Warning;
					}
					GLogLevel = level;
				}
				catch (std::exception) { Error("Log level must be an integer."); }
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
				GQuiet = true;
			}
		}
	}
	
	return output;
}

void ShowHelp()
{
	Console(OUT_COLOR R"(Cinder is a modular and easy to extend raytracer.

Usage: Cinder [option/filename] [option/filename]...
Options: 
  -help/-h               What you are looking at right now.
  -threads/-t=<n>        Number of threads to use for rendering.
                         Do not specify to auto-detect from the number of cores available.
  -nologo                Suppress the banner on startup.
  -quiet/-q              Suppress everything except fatal errors. Implies -nologo.
  -loglevel/-l=<n>       <n> must be from 0 - 4:
                         0 - Verbose
                         1 - Log
                         2 - Warning
                         3 - Error
                         4 - Fatal
)" OUT_RESET);
}
