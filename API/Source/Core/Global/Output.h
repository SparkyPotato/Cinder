//    Copyright 2021 SparkyPotato
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

#pragma once

#include "fmt/color.h"
#include "fmt/core.h"

#include "Interface.h"

extern fmt::memory_buffer GFormatBuffer;

extern const char* GNewLineStart;
extern const char* GNewLineEnd;
extern std::string GDebug;
extern std::string GVerbose;
extern std::string GLog;
extern std::string GWarning;
extern std::string GError;
extern std::string GFatal;

extern Options GOptions;

template<typename... Args>
inline void Verbose(std::string format, Args&&... args)
{
	GFormatBuffer.append(GVerbose);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GOptions.LogFile) { fmt::print(GOptions.LogFile, GFormatBuffer.data()); }
	
	if (GOptions.LoggingLevel > LL_Verbose) { GFormatBuffer.clear(); return; }
	
	if (GOptions.VerboseCallback) { GOptions.VerboseCallback(GFormatBuffer.data()); }
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Log(std::string format, Args&&... args)
{
	GFormatBuffer.append(GLog);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GOptions.LogFile) { fmt::print(GOptions.LogFile, GFormatBuffer.data()); }
	
	if (GOptions.LoggingLevel > LL_Log) { GFormatBuffer.clear(); return; }
	
	if (GOptions.LogCallback) { GOptions.LogCallback(GFormatBuffer.data()); }
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Warning(std::string format, Args&&... args)
{
	GFormatBuffer.append(GWarning);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GOptions.LogFile) { fmt::print(GOptions.LogFile, GFormatBuffer.data()); }
	
	if (GOptions.LoggingLevel > LL_Warning) { GFormatBuffer.clear(); return; }
	
	if (GOptions.WarningCallback) { GOptions.WarningCallback(GFormatBuffer.data()); }
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Error(std::string format, Args&&... args)
{
	GFormatBuffer.append(GError);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GOptions.LogFile) { fmt::print(GOptions.LogFile, GFormatBuffer.data()); }
	
	if (GOptions.LoggingLevel > LL_Error) { GFormatBuffer.clear(); return; }
	
	if (GOptions.ErrorCallback) { GOptions.ErrorCallback(GFormatBuffer.data()); }
	GFormatBuffer.clear();
}

template<typename... Args>
inline void Fatal(std::string format, Args&&... args)
{
	GFormatBuffer.append(GFatal);
	fmt::format_to(GFormatBuffer, format, std::forward<Args>(args)...);
	GFormatBuffer.append(GNewLineStart, GNewLineEnd);
	if (GOptions.LogFile) { fmt::print(GOptions.LogFile, GFormatBuffer.data()); }
	
	if (GOptions.FatalCallback) { GOptions.FatalCallback(GFormatBuffer.data()); }

	throw -1;
}
