#include "Options.h"

#define OUT_CONSOLE "\x1b[97m"
#define OUT_VERBOSE "\x1b[37m"
#define OUT_LOG "\x1b[92m"
#define OUT_WARNING "\x1b[93m"
#define OUT_ERROR "\x1b[91m"
#define OUT_FATAL "\x1b[31m"
#define OUT_COLOR "\x1b[38;2;255;140;0m"
#define OUT_RESET "\x1b[0m"

#ifdef PLATFORM_WINDOWS

std::string ToUTF8(const wchar_t* utf16);

#endif

inline void Console(const char* format)
{
	if (GQuiet) { return; }

	printf(OUT_CONSOLE "%s\n" OUT_RESET, format);
}
template<typename... Args>
void Console(const char* format, Args&&... args)
{
	if (GQuiet) { return; }

	printf(OUT_CONSOLE);
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");
}

inline void Verbose(const char* format)
{
	if (GQuiet || GLogLevel > LogLevel::Verbose) { return; }

	printf(OUT_VERBOSE "Verbose: %s\n" OUT_RESET, format);
}
template<typename... Args>
void Verbose(const char* format, Args&&... args)
{
	if (GQuiet || GLogLevel > LogLevel::Verbose) { return; }

	printf(OUT_VERBOSE "Verbose: ");
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");
}

inline void Log(const char* format)
{
	if (GQuiet || GLogLevel > LogLevel::Log) { return; }

	printf(OUT_LOG "Log: %s\n" OUT_RESET, format);
}
template<typename... Args>
void Log(const char* format, Args&&... args)
{
	if (GQuiet || GLogLevel > LogLevel::Log) { return; }

	printf(OUT_LOG "Log: ");
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");
}

inline void Warning(const char* format)
{
	if (GQuiet || GLogLevel > LogLevel::Warning) { return; }

	printf(OUT_WARNING "Warning: %s\n" OUT_RESET, format);
}
template<typename... Args>
void Warning(const char* format, Args&&... args)
{
	if (GQuiet || GLogLevel > LogLevel::Warning) { return; }

	printf(OUT_WARNING "Warning: ");
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");
}

inline void Error(const char* format)
{
	if (GQuiet || GLogLevel > LogLevel::Error) { return; }

	printf(OUT_ERROR "Error: %s\n" OUT_RESET, format);
}
template<typename... Args>
void Error(const char* format, Args&&... args)
{
	if (GQuiet || GLogLevel > LogLevel::Error) { return; }

	printf(OUT_ERROR "Error: ");
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");
}

inline void Fatal(const char* format)
{
	printf(OUT_FATAL "Fatal: %s\n" OUT_RESET, format);

	throw - 1;
}
template<typename... Args>
void Fatal(const char* format, Args&&... args)
{
	printf(OUT_WAFATAL "Fatal: ");
	printf(format, std::forward<Args...>(args...));
	printf(OUT_RESET "\n");

	throw - 1;
}
