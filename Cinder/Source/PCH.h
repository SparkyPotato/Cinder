#pragma once

#ifdef _WIN32
#	ifdef _WIN64
#		define PLATFORM_WINDOWS
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

#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Global/Assert.h"
#include "Global/Options.h"
#include "Global/Output.h"
#include "Math/Math.h"
