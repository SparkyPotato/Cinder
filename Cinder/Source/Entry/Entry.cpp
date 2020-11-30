#include "PCH.h"

#include "Core/Global/Options.h"
#include "Run.h"

int Entry(int argc, char** argv)
{
	try
	{
		std::vector<const char*> projectFiles;
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
			else if (*argv[i] != '-') { projectFiles.emplace_back(argv[i]); }
			else { optionsInput.emplace_back(argv[i]); }
		}
		
#ifdef CFG_DEBUG
		CinderColored(R"(** DEBUG BUILD **
Will be slow, set log level to 0 to see debug information.
		)");
#endif
		
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
		
		Log("Rendering complete.");
		
		fclose(GLogFile);
		return EXIT_SUCCESS;
	}
	catch (std::exception& e) { Console(e.what()); }
	catch (...) {}
	
	fclose(GLogFile);
	return EXIT_FAILURE;
}
