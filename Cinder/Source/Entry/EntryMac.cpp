#include "PCH.h"

#ifdef PLATFORM_MAC

int main(int argc, char** argv)
{
	std::vector<const char* const> configFiles;
	// Parse command line arguments
	for (int i = 0; i < argc; i++)
	{
		// Check if the first character of the argument is a '-',
		// if it is not then we assume it is a config file
		if (*argv[i] != '-')
		{
			configFiles.emplace_back(argv[i]);
		}
		else
		{
			
		}
	}
	
	return 0;
}

#endif
