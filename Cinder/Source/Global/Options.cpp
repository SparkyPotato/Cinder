#include "PCH.h"
#include "Options.h"

void ShowHelp();

Options GenerateOptions(const std::vector<std::string>& options)
{
	Options output;
	for (auto option : options)
	{
		size_t i = 0;
		if ((i = option.find('=')) != option.npos)
		{
			// Option has a value
			auto name = option.substr(0, i);
			auto value = option.substr(i + 1);
			if (value.empty()) { Error("Value of option '%s' cannot be empty.", name.c_str()); }
			
			if (name == "-threads" || name == "-t")
			{
				try
				{
					int count = stoi(value);
					if (count <= 0) { Error("ThreadCount cannot be 0 or negative."); }
					output.ThreadCount = count;
					if (std::thread::hardware_concurrency() > 0 && output.ThreadCount > std::thread::hardware_concurrency() * 2)
					{
						Warning("ThreadCount is much greater than the maximum concurrent threads supported, you might lose some performance.");
					}
				}
				catch (std::exception) { Error("ThreadCount must be an integer."); }
			}
			else
			{
				Warning("Ignoring unrecognized option '%s'.", name.c_str());
			}
		}
		else
		{
			if (option == "-help" || option == "-h")
			{
				ShowHelp();
				exit(0);
			}
			else
			{ Warning("Ignoring unrecognized option '%s'.", option.c_str()); }
		}
	}
	
	return output;
}

void ShowHelp()
{
	Output(OUT_COLOR "The Cinder Raytracer");
	Output("Cinder is a modular, and easy to extend raytracer.");
	Output(OUT_RESET);
}
