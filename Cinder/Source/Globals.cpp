#include "Globals.h"

namespace CommandLine
{
	std::map <std::wstring, std::wstring> Properties;
	std::vector<std::wstring> Switches;
}

Framebuffer::Framebuffer(uint64_t width, uint64_t height)
	: Width(width), Height(height)
{
	Buffer = new Pixel[Width * Height];
}

static std::vector<std::wstring> s_ValidSwitches =
{
	L"output"
};
static std::vector<std::wstring> s_ValidProperties =
{
	L"w", L"h"
};

bool VerifySwitch(const std::wstring& switchArg)
{
	auto it = std::find(s_ValidSwitches.begin(), s_ValidSwitches.end(), switchArg);

	return it != s_ValidSwitches.end();
}

bool VerifyProperty(const std::wstring& propertyName)
{
	auto it = std::find(s_ValidProperties.begin(), s_ValidProperties.end(), propertyName);

	return it != s_ValidProperties.end();
}

void ParseCommandLine(int argc, wchar_t** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::wstring arg = argv[i]; // Convert from UTF-16 to UTF-8

		if (arg[0] == '-') // A '-' indicates a switch
		{
			std::wstring switchArg = arg.substr(1, arg.npos); // Remove the preceding '-'

			for (auto& c : switchArg) // All switches are single-byte, so they can be lowercase-ified with simple addition
			{
				if (c >= 65 && c <= 90) c += 32;
			}

			if (VerifySwitch(switchArg)) { CommandLine::Switches.emplace_back(std::move(switchArg)); }
			else { Warning("Ignoring unknown switch '", switchArg, "'."); }
		}
		else
		{
			size_t pos = arg.find('=', 0); // Find the '=' separator between the property and its value

			if (pos == arg.npos)
			{
				Error("Invalid property specifier: ", arg); // Invalid property if we don't find the '='
			}

			// If the argument is "dir=Engine/"
			std::wstring name = arg.substr(0, pos); // name = "dir"
			std::wstring value = arg.substr(pos + 1, arg.npos); // value = "Engine/"
			//              Skips the '='  -^

			for (auto& c : name) // Property names are also single-byte
			{
				if (c >= 65 && c <= 90) c += 32;
			}
			// Values are not edited in any way, as they can be multi-byte UTF-8 characters (pathnames!)

			if (VerifyProperty(name)) { CommandLine::Properties.emplace(std::move(name), std::move(value)); }
			else { Warning("Ignoring unknown property '", name, "'."); }
		}
	}
}