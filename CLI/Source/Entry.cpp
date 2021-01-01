//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "Options.h"
#include "Output.h"

int Entry(int argc, char** argv)
{
	Options options;

	try
	{
		std::vector<const char*> projectFiles;
		std::vector<std::string> optionsInput;

#ifndef NDEBUG
		fmt::print(fg(fmt::color::dark_orange), R"(** DEBUG BUILD **
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
					fmt::print(fg(fmt::color::yellow), "Warning: Duplicate project '{}'. Skipping.\n", argv[i]);
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
			fmt::print(fg(fmt::color::dark_orange), "Cinder (built {}, {})\n", __TIME__, __DATE__);
			fmt::print(fg(fmt::color::dark_orange), "Copyright 2021 SparkyPotato (under the Apache License).\n");
		}
		options = GenerateOptions(optionsInput);
		options.VerboseCallback = &OutVerbose;
		options.LogCallback = &OutLog;
		options.WarningCallback = &OutWarning;
		options.ErrorCallback = &OutError;
		options.FatalCallback = &OutFatal;
		options.ProgressCallback = &Progress;
		SetOptions(&options);

		if (projectFiles.empty())
		{
			fmt::print(fg(fmt::color::dark_red), "Fatal: No project files passed. \nRun with '-help' or '-h' for help.\n");
			throw -1;
		}
		
		for (auto file : projectFiles)
		{
			std::filesystem::path path = file;
			if (!std::filesystem::exists(path))
			{
				fmt::print(fg(fmt::color::red), "Error: Project file '{}' does not exist! Skipping.\n", path.string());
				continue;
			}
			if (!std::filesystem::is_regular_file(path))
			{
				fmt::print(fg(fmt::color::red), "Error: '{}' is not a file! Skipping.\n", path.string());
				continue;
			}
			
			GBar = ProgressBar(0.f, 100.f, 1.f);
			float time;
			if (!RunCinderProject(path.string().c_str(), &time))
			{
				throw -1;
			}
			GBar.End();

			fmt::print("Took {}s\n", time);
		}
		
		fclose(options.LogFile);
		return EXIT_SUCCESS;
	}
	catch (std::exception& e) { fmt::print(e.what()); } // This shouldn't ever be hit, but if it does we know what the exception was
	catch (...) {}
	
	fclose(options.LogFile);
	return EXIT_FAILURE;
}
