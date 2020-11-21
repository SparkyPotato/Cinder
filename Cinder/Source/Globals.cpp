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
	
};
static std::vector<std::wstring> s_ValidProperties =
{
	L"w", L"h",
	L"output",
	L"scene",
	L"threads"
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
			std::wstring name = arg.substr(0, pos); // name = "dir"
			std::wstring value = arg.substr(pos + 1, arg.npos); // value = "Engine/"
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

Pixel& Pixel::operator=(const Vector& color)
{
	R = uint8_t(std::min(color.X * 255, 255.f));
	G = uint8_t(std::min(color.Y * 255, 255.f));
	B = uint8_t(std::min(color.Z * 255, 255.f));

	return *this;
}
