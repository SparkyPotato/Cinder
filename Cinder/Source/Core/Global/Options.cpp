#include "PCH.h"
#include "Options.h"

void ShowHelp();

int GLogLevel = LogLevel::Warning;
bool GQuiet = false;
FILE* GLogFile = nullptr;
Options GOptions;
fmt::memory_buffer GFormatBuffer;

const char* GNewLineStart = "\r\n\0";
const char* GNewLineEnd = GNewLineStart + 3;
std::string GDebug = "Debug: ";
std::string GVerbose = "Verbose: ";
std::string GLog = "Log: ";
std::string GWarning = "Warning: ";
std::string GError = "Error: ";
std::string GFatal = "Fatal: ";

static std::vector<std::string> s_ValidOptions =
{
	"-threads", "-t",
	"-loglevel", "-ll",
	"-log", "-l"
};
static std::vector<std::string> s_ValidSwitches =
{
	"-help", "-h",
	"-quiet", "-q",
};

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

		if (!isOption && !isSwitch) { Warning("Ignoring unknown option '{}'.", option.c_str()); }

		if (isOption)
		{
			auto value = option.substr(i + 1);
			if (value.empty()) { Error("Value of option '{}' cannot be empty.", name.c_str()); }
			
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
						Warning("Thread count is much greater than the maximum concurrent threads supported ({}), you might lose some performance.",
							std::thread::hardware_concurrency());
					}
				}
				catch (std::exception) { Error("Thread count must be an integer. Auto-detecting."); }
			}
			else if (name == "-loglevel" || name == "-ll")
			{
				try
				{
					int level = stoi(value);
					if (level < 0 || level > 4)
					{
						Error("Log level must be between 0 and 4 (inclusive).");
						level = LogLevel::Warning;
					}
					GLogLevel = level;
				}
				catch (std::exception) { Error("Log level must be an integer."); }
			}
			else if (name == "-log" || name == "-l")
			{
				logFile = value;
				if (!logFile.has_filename())
				{
					Fatal("Log file must be a file, not directory!");
				}

				try { std::filesystem::create_directories(logFile.root_path()); }
				catch (...)
				{
					Fatal("Could not create log file '{}'", value);
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
				GQuiet = true;
				GLogLevel = LogLevel::Fatal;
			}
		}
	}
	
	GLogFile = FileOpen(logFile.string().c_str(), "w");
	if (!GLogFile)
	{
		Error("Could not open log file. File logging is disabled.");
	}

	return output;
}

void ShowHelp()
{
	CinderColored(R"(Cinder is a modular and easy to extend raytracer.

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
)");
}
