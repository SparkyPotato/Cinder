#include "PCH.h"
#include "Run.h"

#include "fmt/chrono.h"

#include "Core/Components/Renderer.h"

void RunProject(const std::filesystem::path& filePath)
{
	std::filesystem::path workingDirectory = std::filesystem::current_path();
	std::filesystem::current_path(filePath.parent_path());

	auto start = std::chrono::high_resolution_clock().now();

	YAML::Node project;
	try { project = YAML::LoadFile(filePath.string()); }
	catch (YAML::Exception& e)
	{
		Error("Failed to parse project file (line {})!", e.mark.line);
		Error("Skipping project.");
		return;
	}

	try { Console("{}", project["Project Name"].as<std::string>()); }
	catch (YAML::Exception& e)
	{
		Error("Error parsing project name (line {})!", e.mark.line + 1);
		return;
	}

	Renderer* renderer = nullptr;
	try { renderer = project["Renderer"].as<Renderer*>(); }
	catch (...) { return; }

	Framebuffer* framebuffer = nullptr;
	try { framebuffer = project["Framebuffer"].as<Framebuffer*>(); }
	catch (...) { return; }

	if (!project["Scenes"].IsSequence())
	{
		Error("Scene list must be a sequence (line {})!", project["Scenes"].Mark().line + 1);
		return;
	}

	for (auto& sceneIt : project["Scenes"])
	{
		Memory::Get()->StartRange();

		std::string file;
		std::string output;
		try { file = sceneIt["File"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Failed to find Scene File (line {})!", e.mark.line + 1);
			return;
		}
		try { output = sceneIt["Output"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Failed to find Scene Ouput (line {})!", e.mark.line + 1);
			return;
		}

		Scene* scene;
		try { scene = Scene::Load(file); }
		catch (...) { return; }
		scene->SetCameraAspectRatio(float(framebuffer->Width) / framebuffer->Height);
		Log("Loaded Scene.");

		renderer->Render(*scene, *framebuffer);
		Log("Rendered.");

		framebuffer->Ouput(output);
		Log("Written to file.");

		delete scene;
	}

	auto end = std::chrono::high_resolution_clock().now();

	Console("Took {:%M:%S}.", end - start);

	delete renderer;
	delete framebuffer;

	std::filesystem::current_path(workingDirectory);
}
