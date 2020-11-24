#include "PCH.h"

#include "Global/Options.h"

int Entry(int argc, char** argv)
{
	try
	{
		std::vector<const char*> configFiles;
		std::vector<std::string> optionsInput;
		
		// Parse command line arguments:
		// 1. All options start with a '-'.
		// 2. They must follow the format '-<option>=<value>'
		// 3. Anything that is not an option is taken as a config file
		// Cinder can take multiple config files,
		// and they will be executed in the order of entry
		bool logo = true;
		for (int i = 1; i < argc; i++)
		{
			// Check if the first character of the argument is a '-',
			// if it is not then we assume it is a config file
			if (strcmp(argv[i], "-nologo") == 0) { logo = false; }
			else if (*argv[i] != '-') { configFiles.emplace_back(argv[i]); }
			else { optionsInput.emplace_back(argv[i]); }
		}

		if (logo)
		{
			Console(OUT_COLOR "The Cinder Raytracer");
			Console(OUT_COLOR "Copyright ©2020 SparkyPotato (under the MIT License). \n" OUT_RESET);
		}
		auto options = GenerateOptions(optionsInput);
		
		if (configFiles.empty())
		{
			Fatal("No config files passed. \nRun with '-help' or '-h' for help.");
		}
		
		if (options.ThreadCount == 0)
		{
			Log("Auto-detecting thread count.");
			options.ThreadCount = std::thread::hardware_concurrency();
			if (options.ThreadCount == 0)
			{
				Warning("Could not detect number of threads, using 4 as a reasonable default.");
				options.ThreadCount = 4;
			}
		}
		
		Log("Rendering with %u threads.", options.ThreadCount);
		
		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		Console(e.what());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
}
