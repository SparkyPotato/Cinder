#include "Globals.h"

#include <Windows.h>

#undef min

namespace CommandLine
{
	std::map<std::string, std::string> Properties;
	std::vector<std::string> Switches;
}

std::string ToUTF8(const wchar_t* string)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, nullptr, 0, nullptr, nullptr);
	std::string temp(size, 0);

	WideCharToMultiByte(CP_UTF8, 0, string, -1, &temp[0], size, nullptr, nullptr);
	temp.pop_back(); // Fixes a weird issue where we have double nulls

	return temp;
}

std::wstring ToUTF16(const std::string& string)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(string.c_str()), (int)string.size(), nullptr, 0);
	std::wstring temp(size, 0);

	MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &temp[0], (int)temp.capacity());

	return temp;
}

Framebuffer::Framebuffer(uint64_t width, uint64_t height)
	: Width(width), Height(height)
{
	Buffer = new Pixel[Width * Height];
}

static std::vector<std::string> s_ValidSwitches =
{
	
};
static std::vector<std::string> s_ValidProperties =
{
	"w", "h",
	"output",
	"scene",
	"threads"
};

bool VerifySwitch(const std::string& switchArg)
{
	auto it = std::find(s_ValidSwitches.begin(), s_ValidSwitches.end(), switchArg);

	return it != s_ValidSwitches.end();
}

bool VerifyProperty(const std::string& propertyName)
{
	auto it = std::find(s_ValidProperties.begin(), s_ValidProperties.end(), propertyName);

	return it != s_ValidProperties.end();
}

void ParseCommandLine(int argc, wchar_t** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = ToUTF8(argv[i]); // Convert from UTF-16 to UTF-8

		if (arg[0] == '-') // A '-' indicates a switch
		{
			std::string switchArg = arg.substr(1, arg.npos); // Remove the preceding '-'

			for (auto& c : switchArg) // All switches are single-byte, so they can be lowercase-ified with simple addition
			{
				if (c >= 65 && c <= 90) c += 32;
			}

			if (VerifySwitch(switchArg)) { CommandLine::Switches.emplace_back(std::move(switchArg)); }
			else { Warning("Ignoring unknown switch '" COLOR, switchArg, "\x1b[93m'."); }
		}
		else
		{
			size_t pos = arg.find('=', 0); // Find the '=' separator between the property and its value

			if (pos == arg.npos)
			{
				Error("Invalid property specifier '" COLOR, arg, "\x1b[31m'."); // Invalid property if we don't find the '='
			}

			// If the argument is "dir=Engine/"
			std::string name = arg.substr(0, pos); // name = "dir"
			std::string value = arg.substr(pos + 1, arg.npos); // value = "Engine/"
			//              Skips the '='  -^

			for (auto& c : name) // Property names are also single-byte
			{
				if (c >= 65 && c <= 90) c += 32;
			}
			// Values are not edited in any way, as they can be multi-byte UTF-8 characters (pathnames!)

			if (VerifyProperty(name)) { CommandLine::Properties.emplace(std::move(name), std::move(value)); }
			else { Warning("Ignoring unknown property '" COLOR, name, "\x1b[93m'."); }
		}
	}
}

ProgressBar::ProgressBar(uint64_t min, uint64_t max, uint64_t step)
	: m_Min(min), m_Max(max), m_BarValue(min), m_Step(step), m_RealValue(min)
{
	printf(COLOR "%llu ", m_RealValue);
}

void ProgressBar::Update(uint64_t value)
{
	if (value > m_RealValue)
	{
		int64_t input = value;
		int64_t realValue = m_RealValue;

		printf("\x1b[1G" COLOR); // Go to the first character

		// Remove all the digits of the old value
		int i = 0;
		while (realValue > 0) { i++; realValue /= 10; }
		printf("\x1b[%dP", i);

		// Create space for the digits of the new value
		i = 0;
		while (input > 0) { i++;  input /= 10; }
		printf("\x1b[%d@", i);

		printf("%llu ", value); // Print the new value

		// Add to the bar
		if ((value - m_BarValue) >= m_Step)
		{
			input = value;
			while (input > int64_t(m_BarValue))
			{
				printf("\x1b[1@");
				printf("█");
				input -= m_Step;
			}
			m_BarValue = value;
		}
		m_RealValue = value;

		printf("\x1b[0m");
	}
}

void ProgressBar::End()
{
	Update(m_Max);
	printf("\n");
}
