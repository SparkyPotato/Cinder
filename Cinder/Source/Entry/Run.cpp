#include "PCH.h"
#include "Run.h"

#include "fmt/chrono.h"

#include "Core/Components/Renderer.h"

#include "Geometry/Sphere.h"

void RunProject(const std::filesystem::path& filePath)
{
	auto start = std::chrono::high_resolution_clock().now();

	YAML::Node project;
	try { project = YAML::LoadFile(filePath.string()); }
	catch (YAML::Exception& e)
	{
		Error("Failed to parse project file (line {})!", e.mark.line);
		Error("Skipping project.");
		return;
	}

	// Set the working directory to the directory containing the project,
	// so we can use relative paths without issues.
	std::filesystem::path workingDirectory = std::filesystem::current_path();
	std::filesystem::current_path(filePath.parent_path());

	try { Console("{}", project["Project Name"].as<std::string>()); }
	catch (YAML::Exception& e)
	{
		Error("Error parsing project name (line {})!", e.mark.line + 1);
		return;
	}

	up<Renderer> renderer = nullptr;
	try { renderer = project["Renderer"].as<up<Renderer>>(); }
	catch (...) { return; }

	up<Framebuffer> framebuffer = nullptr;
	try { framebuffer = project["Framebuffer"].as<up<Framebuffer>>(); }
	catch (...) { return; }
	
	// Cinder supports a list of Scenes to render per project
	if (!project["Scenes"].IsSequence())
	{
		Error("Scene list must be a sequence (line {})!", project["Scenes"].Mark().line + 1);
		return;
	}

	for (const auto& sceneIt : project["Scenes"])
	{
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
		
		// Load the scene and time it
		auto s = std::chrono::high_resolution_clock().now();
		Log("Loading Scene.");
		Scene* scene;
		try { scene = Scene::Load(file); }
		catch (YAML::Exception& e) { Error(e.what()); return; }
		scene->SetCameraAspectRatio(float(framebuffer->Width) / framebuffer->Height);
		auto e = std::chrono::high_resolution_clock().now();
		Log("Loaded Scene. Took {:%M:%S}.", e - s);
		
		// Render and time it
		s = std::chrono::high_resolution_clock().now();
		Log("Rendering.");
		renderer->Render(*scene, *framebuffer);
		e = std::chrono::high_resolution_clock().now();
		Log("Rendered. Took {:%M:%S}.", e - s);
		
		// Output and time it
		s = std::chrono::high_resolution_clock().now();
		Log("Writing output.");
		framebuffer->Ouput(output);
		e = std::chrono::high_resolution_clock().now();
		Log("Written output. Took {:%M:%S}.", e - s);

		delete scene;
	}

	auto end = std::chrono::high_resolution_clock().now();

	Console("Took {:%M:%S}.", end - start);
	
	// Reset the working directory so the next project can be loaded
	std::filesystem::current_path(workingDirectory);
}
