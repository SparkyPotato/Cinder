#include "PCH.h"

#include "Assert.h"

void Assert(bool condition, const char* message, const char* file, int line)
{
	if (!condition)
	{
		Fatal("Assertion Failed - {} \nAt line: {}, file: {}", message, line, file);
	}
}
