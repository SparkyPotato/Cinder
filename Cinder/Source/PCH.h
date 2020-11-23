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
#		error macOS is not supported yet!
#	else
#		error Unknown Apple platform!
#	endif
#elif defined(__ANDROID__)
#	error Android is not supported!
#elif defined(__linux__)
#	error Linus is not supported yet
#else
#	error Unknown platform!
#endif
