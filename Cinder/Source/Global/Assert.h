#pragma once

#ifdef CFG_DEBUG
#	define ASSERT(condition, message) Assert(condition, message, __FILE__, __LINE__)
#else
#	define ASSERT(condition, message)
#endif

void Assert(bool condition, const char* message, const char* file, int line);
