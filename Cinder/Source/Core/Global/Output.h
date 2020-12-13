#pragma once

#include "fmt/color.h"
#include "fmt/core.h"

#include "Core/Global/Options.h"

extern fmt::memory_buffer GFormatBuffer;

extern const char* GNewLineStart;
extern const char* GNewLineEnd;
extern std::string GDebug;
extern std::string GVerbose;
extern std::string GLog;
extern std::string GWarning;
extern std::string GError;
extern std::string GFatal;
extern std::mutex GOutputMutex;

template<typename... Args>
inline void Console(const char* format, Args&&... args)
{
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	
	if (GQuiet) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void CinderColored(std::string format, Args&&... args)
{
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	
	if (GQuiet) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::dark_orange), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Debug(std::string format, Args&&... args)
{
#ifndef NDEBUG
	GFormatBuffer.append(GDebug);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	if (GLogLevel > LogLevel::Verbose) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::dark_gray), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
#endif
}

template<typename... Args>
inline void Verbose(std::string format, Args&&... args)
{
	GFormatBuffer.append(GVerbose);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	if (GLogLevel > LogLevel::Verbose) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::light_gray), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Log(std::string format, Args&&... args)
{
	GFormatBuffer.append(GLog);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	if (GLogLevel > LogLevel::Log) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::light_green), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Warning(std::string format, Args&&... args)
{
	GFormatBuffer.append(GWarning);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	if (GLogLevel > LogLevel::Warning) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::yellow), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Error(std::string format, Args&&... args)
{
	GFormatBuffer.append(GError);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	if (GLogLevel > LogLevel::Error) { GFormatBuffer.clear(); return; }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::red), GFormatBuffer.data());
	GOutputMutex.unlock();
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Fatal(std::string format, Args&&... args)
{
	GFormatBuffer.append(GFatal);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GLogFile) { fmt::print(GLogFile, GFormatBuffer.data()); }
	
	GOutputMutex.lock();
	fmt::print(fg(fmt::color::dark_red), GFormatBuffer.data());
	GOutputMutex.unlock();

	throw -1;
}
