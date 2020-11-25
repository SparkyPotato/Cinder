#include "PCH.h"

#include "Assert.h"

void Assert(bool condition, char* message, char* file, int line)
{
	if (!condition)
	{
		Fatal("Assertion Failed - %s \nAt line: %d, file: %s", message, line, file);
	}
}
