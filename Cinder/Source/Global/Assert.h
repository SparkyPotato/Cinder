#pragma once

#ifdef PLATFORM_WINDOWS

#	ifdef CFG_DEBUG
#		define ASSERT(condition, message) __debugbreak(); Assert(condition, message, __FILE__, __LINE__)
#	else
#		define ASSERT(condition, message)
#	endif

#else

#	ifdef CFG_DEBUG
#		define ASSERT(condition, message) Assert(condition, message, __FILE__, __LINE__)
#	else
#		define ASSERT(condition, message)
#	endif

#endif

void Assert(bool condition, const char* message, const char* file, int line);
