#include "PCH.h"

int Entry(int argc, char** argv);

#ifndef PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	return Entry(argc, argv);
}

#endif
