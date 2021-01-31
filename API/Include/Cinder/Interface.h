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

#pragma once

#include <stdint.h>

#ifdef _WIN32
#	ifdef _WIN64
#		define PLATFORM_WINDOWS
#		ifdef BUILD_CINDER
#			define CINDER_API __declspec(dllexport)
#		else
#			define CINDER_API __declspec(dllimport)
#		endif
#	else
#		error 32 bit is not supported!
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error iOS simulator not supported!
#	elif TARGET_OS_IPHONE == 1
#		error iOS is not supported!
#	elif TARGET_OS_MAC == 1
#		define PLATFORM_MAC
#	else
#		error Unknown Apple platform!
#	endif
#elif defined(__ANDROID__)
#	error Android is not supported!
#elif defined(__linux__)
#	define PLATFORM_LINUX
#else
#	error Unknown platform!
#endif

#ifndef CINDER_API
# 	define CINDER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Allowed logging levels.
enum LogLevel
{
	LL_Verbose = 0,
	LL_Log = 1,
	LL_Warning = 2,
	LL_Error = 3,
	LL_Fatal = 4
};

/// Options for Cinder.
struct Options
{
	/// The log level to use.
	LogLevel LoggingLevel = LL_Warning;

	/// The file in which to output logs.
	FILE* LogFile = NULL;

	/// Whether Cinder should be completely quiet.
	bool Quiet = false;

	/// Callbacks for various loggers.
	void (*VerboseCallback)(const char*) = nullptr;
	void (*LogCallback)(const char*) = nullptr;
	void (*WarningCallback)(const char*) = nullptr;
	void (*ErrorCallback)(const char*) = nullptr;
	void (*FatalCallback)(const char*) = nullptr;

	/// Number of threads to use. 0 will auto-detect.
	uint16_t ThreadCount = 0;

	/// The size of each square render tile.
	uint32_t TileSize = 16;

	/// Callback for rendering progress. May be called many times in one second.
	void (*ProgressCallback)(float) = nullptr;
};

/// Set the options to be used by Cinder.
///
/// \param options The options to set.
/// 
/// \return If setting the options was successful.
CINDER_API bool SetOptions(Options* options);

/// Run a Cinder project.
///
/// \param filePath Path of the project file.
/// \param timeSeconds The time the render took (Is filled by Cinder).
CINDER_API bool RunCinderProject(const char* filePath, float* timeSeconds);

#ifdef __cplusplus
}
#endif
