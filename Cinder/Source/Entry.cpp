#include <iomanip>
#include <Windows.h>

#include "json/json.hpp"
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Globals.h"
#include "Scene.h"

void ParseCommandLine(int argc, wchar_t** argv);

int wmain(int argc, wchar_t** argv)
{
	try
	{
		ParseCommandLine(argc, argv);

		std::wstring output = L"output.png";
		if (CommandLine::Properties.count(L"output")) { output = CommandLine::Properties[L"output"]; }
		std::wstring scenePath = L"scene.json";
		if (CommandLine::Properties.count(L"scene")) { output = CommandLine::Properties[L"scene"]; }

		nlohmann::json j;
		try { std::ifstream(scenePath) >> j; }
		catch (...) { Error("Failed to parse scene file '", scenePath, "'."); }

		auto scene = j.get<Scene>();

		// Framebuffer creation, and each pixel to black
		uint64_t framebufferWidth = 1920;
		uint64_t framebufferHeight = 1080;
		if (CommandLine::Properties.count(L"w")) { framebufferWidth = stoull(CommandLine::Properties[L"w"]); }
		if (CommandLine::Properties.count(L"h")) { framebufferHeight = stoull(CommandLine::Properties[L"h"]); }
		auto framebuffer = new Pixel[framebufferWidth * framebufferHeight];
		memset(framebuffer, 0, framebufferWidth * framebufferHeight * sizeof(Pixel));

		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		Output("Starting Render");

		for (uint64_t y = 0; y < framebufferHeight; y++)
		{
			for (uint64_t x = 0; x < framebufferWidth; x++)
			{

			}
			Output("Completed ", y * 100/ framebufferHeight, "%.");
		}

		QueryPerformanceCounter(&end);
		auto time = float(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;

		Output("Render Complete. Took ", std::fixed, std::setprecision(2), time, "s.");
		Output("Writing to file '", output, "' with format PNG.");

		int size = WideCharToMultiByte(CP_UTF8, 0, output.c_str(), (int)output.size(), nullptr, 0, nullptr, nullptr);
		auto utf8 = new char[size];
		WideCharToMultiByte(CP_UTF8, 0, output.c_str(), (int)output.size(), utf8, size, nullptr, nullptr);

		stbi_write_png(utf8, (int)framebufferWidth, (int)framebufferHeight, 3, framebuffer, (int)framebufferWidth * 3);

		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		Output(e.what());
	}
	catch (...)
	{}

	return EXIT_FAILURE;
}

static std::vector<std::wstring> s_ValidSwitches =
{
	
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
