#include <iomanip>
#include <Windows.h>

#include "json/json.hpp"
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Globals.h"
#include "Raytracer.h"
#include "Scene.h"

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

		Output("Starting Render");

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
