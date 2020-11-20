#include <iomanip>
#include <Windows.h>

#include "json/json.hpp"
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Globals.h"
#include "Raytracer.h"
#include "Scene.h"

void SetupConsole();

int wmain(int argc, wchar_t** argv)
{
	try
	{
		SetupConsole();
		printf("\x1b[?25l");

		ParseCommandLine(argc, argv);

		std::wstring output = L"output.png";
		if (CommandLine::Properties.count(L"output")) { output = CommandLine::Properties[L"output"]; }
		std::wstring scenePath = L"scene.json";
		if (CommandLine::Properties.count(L"scene")) { scenePath = CommandLine::Properties[L"scene"]; }

		nlohmann::json j;
		try { std::ifstream(scenePath) >> j; }
		catch (std::exception& e) { Error("Failed to parse scene file '", scenePath, "': ", e.what()); }

		auto scene = j.get<Scene>();

		// Framebuffer creation, and set each pixel to black
		uint64_t framebufferWidth = 1920;
		uint64_t framebufferHeight = 1080;
		if (CommandLine::Properties.count(L"w")) { framebufferWidth = stoull(CommandLine::Properties[L"w"]); }
		if (CommandLine::Properties.count(L"h")) { framebufferHeight = stoull(CommandLine::Properties[L"h"]); }
		Framebuffer framebuffer(framebufferWidth, framebufferHeight);
		memset(framebuffer.Buffer, 0, framebuffer.Width * framebuffer.Height * sizeof(Pixel));

		Raytracer raytracer(scene, framebuffer);

		Output("Starting Render.");

		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		raytracer.Run();

		QueryPerformanceCounter(&end);
		auto time = float(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;

		Output("Render Complete. Took ", std::fixed, std::setprecision(2), time, "s.");
		Output("Writing to file '", output, "'.");

		int size = WideCharToMultiByte(CP_UTF8, 0, output.c_str(), -1, nullptr, 0, nullptr, nullptr);
		auto utf8 = new char[size];
		WideCharToMultiByte(CP_UTF8, 0, output.c_str(), -1, utf8, size, nullptr, nullptr);

		stbi_write_png(utf8, (int)framebuffer.Width, (int)framebuffer.Height, 3, framebuffer.Buffer, (int)framebuffer.Width * 3);

		printf("\x1b[?25h");
		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		Output(e.what());
	}
	catch (...)
	{}

	printf("\x1b[?25h");
	return EXIT_FAILURE;
}

void SetupConsole()
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	if (output == INVALID_HANDLE_VALUE)
	{
		printf("Error: Failed to get console handle! Something is really wrong here... \n");
		throw - 1;
	}

	DWORD outMode;
	if (!GetConsoleMode(output, &outMode))
	{
		printf("Error: Could not get console mode! This should not be happening... \n");
		throw -1;
	}

	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(output, outMode))
	{
		printf("Error: Failed to set console output mode! That was unexpected... \n");
		throw - 1;
	}
}
