#include <Windows.h>

#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Globals.h"
#include "Raytracer.h"
#include "Scene/Scene.h"

void SetupConsole();

int wmain(int argc, wchar_t** argv)
{
	try
	{
		SetupConsole();
		printf("\x1b[?25l");

		ParseCommandLine(argc, argv);

		std::string output = "output.png";
		if (CommandLine::Properties.count("output")) { output = CommandLine::Properties["output"]; }

		std::string scenePath = "scene.yaml";
		if (CommandLine::Properties.count("scene")) { scenePath = CommandLine::Properties["scene"]; }

		if (!fs::exists(scenePath)) { Error("Could not find file '" COLOR, fs::absolute(scenePath).wstring(), "\x1b[31m'."); }
		Output("Loading scene from file '" COLOR, fs::absolute(scenePath).wstring(), "\x1b[0m'.");

		Scene scene;
		try { scene = Scene::FromFile(scenePath); }
		catch (std::exception& e) { Error("Couldn't parse file: " COLOR, e.what(), "\x1b[31m."); }

		Output("Loaded scene.");

		// Framebuffer creation
		uint64_t framebufferWidth = 1920;
		uint64_t framebufferHeight = 1080;
		if (CommandLine::Properties.count("w")) { framebufferWidth = stoull(CommandLine::Properties["w"]); }
		if (CommandLine::Properties.count("h")) { framebufferHeight = stoull(CommandLine::Properties["h"]); }
		Framebuffer framebuffer(framebufferWidth, framebufferHeight);

		Raytracer raytracer(scene, framebuffer);

		Output("Starting Render. \n");

		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		raytracer.Run();

		QueryPerformanceCounter(&end);
		auto time = float(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;

		Output(COLOR "Render Complete. Took ", std::fixed, std::setprecision(2), time, "s.\x1b[0m");
		Output("Writing to file '" COLOR, output, "\x1b[0m'.");

		stbi_write_png(output.c_str(), (int)framebuffer.Width, (int)framebuffer.Height, 3, framebuffer.Buffer, (int)framebuffer.Width * 3);

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
