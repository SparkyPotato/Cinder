#include "PCH.h"

#include "Core/Global/Options.h"
#include "Run.h"

int Entry(int argc, char** argv)
{
	try
	{
		std::vector<const char*> projectFiles;
		std::vector<std::string> optionsInput;

#ifndef NDEBUG
		CinderColored(R"(** DEBUG BUILD **
Will be slow, set log level to 0 to see debug information.
		)");
#endif

		// Parse command line arguments:
		// 1. All options start with a '-'.
		// 2. They must follow the format '-<option>=<value>'
		// 3. Anything that is not an option is taken as a project file
		// Cinder can take multiple project files, and they will be rendered in the order of entry
		bool logo = true;
		for (int i = 1; i < argc; i++)
		{
			// Check if the first character of the argument is a '-',
			// if it is not then we assume it is a config file
			if (strcmp(argv[i], "-nologo") == 0 || strcmp(argv[i], "-quiet") == 0 || strcmp(argv[i], "-q") == 0) { logo = false; }
			else if (*argv[i] != '-') 
			{ 
				if (std::find(projectFiles.begin(), projectFiles.end(), std::string(argv[i])) != projectFiles.end())
				{
					Warning("Duplicate project '{}'. Skipping.", argv[i]);
				}
				else { projectFiles.emplace_back(argv[i]); }
			}
			else 
			{
				optionsInput.emplace_back(argv[i]);
			}
		}
		
		if (logo)
		{
			CinderColored("Cinder (built {}, {})", __TIME__, __DATE__);
			CinderColored("Copyright 2020 SparkyPotato (under the MIT License).\n");
		}
		GOptions = GenerateOptions(optionsInput);

		if (projectFiles.empty())
		{
			Fatal("No project files passed. \nRun with '-help' or '-h' for help.");
		}
		
		if (GOptions.ThreadCount == 0)
		{
			Log("Auto-detecting thread count.");
			GOptions.ThreadCount = std::thread::hardware_concurrency();
			// hardware_concurrency can return 0 if it failed to get the thread count from the OS
			if (GOptions.ThreadCount == 0)
			{
				Warning("Could not detect number of threads, using 4 as a reasonable default.");
				GOptions.ThreadCount = 4;
			}
		}
		
		Log("Rendering with {} threads.", GOptions.ThreadCount);
		
		for (auto file : projectFiles)
		{
			std::filesystem::path path = file;
			if (!std::filesystem::exists(path))
			{
				Error("Project file '{}' does not exist! Skipping.", path.string());
				continue;
			}
			if (!std::filesystem::is_regular_file(path))
			{
				Error("'{}' is not a file! Skipping.", path.string());
				continue;
			}
			
			RunProject(path);
		}
		
		Log("Complete.");
		
		fclose(GLogFile);
		return EXIT_SUCCESS;
	}
	catch (std::exception& e) { Console(e.what()); } // This shouldn't ever be hit, but if it does we know what the exception was
	catch (...) {}
	
	fclose(GLogFile);
	return EXIT_FAILURE;
}
