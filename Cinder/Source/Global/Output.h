#include "fmt/color.h"
#include "fmt/core.h"

#include "Options.h"

#define OUT_CONSOLE "\x1b[97m"
#define OUT_VERBOSE "\x1b[37m"
#define OUT_LOG "\x1b[92m"
#define OUT_WARNING "\x1b[93m"
#define OUT_ERROR "\x1b[91m"
#define OUT_FATAL "\x1b[31m"
#define OUT_COLOR "\x1b[38;2;255;140;0m"

#ifdef PLATFORM_WINDOWS

std::string ToUTF8(const wchar_t* utf16);

#endif

template<typename... Args>
inline void Console(std::string format, Args&&... args)
{
	if (GQuiet) { return; }

	fmt::print(fg(fmt::color::floral_white),
		format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void CinderColored(std::string format, Args&&... args)
{
	if (GQuiet) { return; }

	fmt::print(fg(fmt::color::dark_orange),
		format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void Verbose(std::string format, Args&&... args)
{
	if (GLogLevel > LogLevel::Verbose) { return; }

	fmt::print(fg(fmt::color::light_gray),
		"Verbose: " + format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void Log(std::string format, Args&&... args)
{
	if (GLogLevel > LogLevel::Log) { return; }

	fmt::print(fg(fmt::color::light_green),
		"Log: " + format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void Warning(std::string format, Args&&... args)
{
	if (GLogLevel > LogLevel::Warning) { return; }

	fmt::print(fg(fmt::color::yellow),
		"Warning: " + format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void Error(std::string format, Args&&... args)
{
	if (GLogLevel > LogLevel::Error) { return; }

	fmt::print(fg(fmt::color::red),
		"Error: " + format + "\n", std::forward<Args>(args)...);
}

template<typename... Args>
inline void Fatal(std::string format, Args&&... args)
{
	fmt::print(fg(fmt::color::dark_red),
		"Fatal: " + format + "\n", std::forward<Args>(args)...);

	throw - 1;
}
