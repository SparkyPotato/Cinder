#include "PCH.h"
#include "Run.h"

#include "fmt/chrono.h"

#include "Core/Components/Renderer.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Components/OutputAdapter.h"
#include "Core/Components/AccelerationStructure.h"

Renderer* SpawnRenderer(YAML::Node& project);
Framebuffer* SpawnFramebuffer(YAML::Node& project);
OutputAdapter* SpawnOutputAdapter(YAML::Node& project);

void RunProject(const std::filesystem::path& filePath)
{
	Console("{}", filePath.filename().string());
	Memory::Get()->StartProject();

	auto setupStart = std::chrono::high_resolution_clock().now();

	YAML::Node project;
	try { project = YAML::LoadFile(filePath.string()); }
	catch (YAML::Exception& e)
	{
		Error("Failed to parse project file (line {})!", e.mark.line);
		Error("Skipping project.");
		return;
	}
	
	std::filesystem::path workingDirectory = std::filesystem::current_path();
	std::filesystem::current_path(filePath.parent_path());

	Renderer* renderer = SpawnRenderer(project);
	if (!renderer)
	{
		Error("Failed Renderer creation. Skipping project.");
		return;
	}

	Framebuffer* framebuffer = SpawnFramebuffer(project);
	if (!framebuffer)
	{
		Error("Failed Framebuffer creation. Skipping project.");
		return;
	}

	OutputAdapter* output = SpawnOutputAdapter(project);
	if (!output)
	{
		Error("Failed Output Adapter creation. Skipping project.");
		return;
	}

	std::string file;
	if (project["Scene"]["File"])
	{
		try { file = project["Scene"]["File"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Scene File must be a string (line {})!", e.mark.line + 1);
			Error("Failed to load Scene. Skipping project.");
			return;
		}
	}
	else
	{
		Error("Scene Path does not exist (line {})!.", project["Scene"].Mark().line + 1);
		Error("Skipping project.");
	}

	Scene* scene;
	try { scene = Scene::FromFile(file); }
	catch (...)
	{
		Error("Failed to load scene!");
		Error("Skipping project.");
		return;
	}

	std::string accelerationStructure = "None";
	if (project["Scene"]["Acceleration"])
	{
		try { accelerationStructure = project["Scene"]["Acceleration"]["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Acceleration Structure Type must be a string (line {})!", e.mark.line + 1);
			Error("Failed to create Acceleration Structure. Skipping project.");
			return;
		}
	}
	else
	{
		Warning("No Acceleration Structure given, using default ({})", accelerationStructure);
	}

	try { scene->AccelStructure = ComponentManager::Get()->SpawnAccelerationStructure(accelerationStructure); }
	catch (...)
	{
		Error("Acceleration Structure '{}' does not exist!", accelerationStructure);
		Error("Failed to create Acceleration Structure. Skipping project.");
		return;
	}
	scene->AccelStructure->Build(*scene);
	scene->MainCamera->SetAspectRatio((float)framebuffer->Width / framebuffer->Height);

	auto setupEnd = std::chrono::high_resolution_clock().now();

	renderer->Render(*scene, *framebuffer);

	auto renderEnd = std::chrono::high_resolution_clock().now();

	output->WriteOutput(*framebuffer);
	Console("Project '{}' finished rendering.", filePath.filename().string());
	Console("Took {:%M:%S}.", renderEnd - setupStart);
	Log("Out of that, load took {:%M:%S}, and render took {:%M:%S}.", setupEnd - setupStart, renderEnd - setupEnd);
	
	delete scene;

	std::filesystem::current_path(workingDirectory);
}

Renderer* SpawnRenderer(YAML::Node& project)
{
	std::string type = "Preview";
	if (project["Renderer"]["Type"]) 
	{ 
		try { type = project["Renderer"]["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Renderer Type must be a string (line {})!", e.mark.line + 1);
			return nullptr;
		}
	}
	else 
	{ 
		Warning("No Renderer Type, using default '{}'.", type); 
	}

	Renderer* renderer = nullptr;
	try { renderer = ComponentManager::Get()->SpawnRenderer(type); }
	catch (...) 
	{
		Error("Renderer Type '{}' does not exist!", type); 
		return nullptr;
	}

	if (!renderer->ParseSettings(project["Renderer"])) 
	{
		Error("Renderer Parse settings failed!");
		return nullptr;
	}

	return renderer;
}

Framebuffer* SpawnFramebuffer(YAML::Node& project)
{
	std::string type = "Simple";
	uint32_t width = 1920, height = 1080;
	if (project["Framebuffer"]["Type"]) 
	{ 
		try { type = project["Framebuffer"]["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Framebuffer Type must be a string (line {})!", e.mark.line + 1);
			return nullptr;
		}
	}
	else 
	{ 
		Warning("No Framebuffer Type, using default ({}).", type);
	}

	if (project["Framebuffer"]["Width"]) 
	{ 
		try { width = project["Framebuffer"]["Width"].as<uint32_t>(); }
		catch (YAML::Exception& e)
		{
			Error("Framebuffer Width must be an integer (line {})!", e.mark.line + 1);
		}
	}
	else 
	{ 
		Warning("No Framebuffer Width, using default ({}).", width);
	}

	if (project["Framebuffer"]["Height"]) 
	{ 
		try { height = project["Framebuffer"]["Height"].as<uint32_t>(); }
		catch (YAML::Exception& e)
		{
			Error("Framebuffer Height must be an integer (line {})!", e.mark.line + 1);
		}
	}
	else 
	{
		Warning("No Framebuffer Height, using default ({}).", width); 
	}

	Framebuffer* framebuffer = nullptr;
	try { framebuffer = ComponentManager::Get()->SpawnFramebuffer(type, width, height); }
	catch (...) 
	{ 
		Error("Framebuffer Type '{}' does not exist!", type);
		return nullptr;
	}

	if (!framebuffer->ParseSettings(project["Framebuffer"])) 
	{ 
		Error("Framebuffer Parse settings failed!");
		return nullptr;
	}

	return framebuffer;
}

OutputAdapter* SpawnOutputAdapter(YAML::Node& project)
{
	std::string type = "PNG";
	std::string file = "Output.png";
	if (project["Output"]["Type"]) 
	{ 
		try { type = project["Output"]["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Output Adapter Type must be a string (line {})!", e.mark.line + 1);
			return nullptr;
		}
	}
	else 
	{ 
		Warning("No Output Type, using default ({})", type); 
	}

	if (project["Output"]["File"]) 
	{ 
		try { file = project["Output"]["File"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Output Adapter File must be a string (line {})!", e.mark.line + 1);
			return nullptr;
		}
	}
	else 
	{ 
		Warning("No Output File, using default ({})", file); 
	}

	OutputAdapter* output = nullptr;
	try { output = ComponentManager::Get()->SpawnOutputAdapter(type, file); }
	catch (...) 
	{ 
		Error("Output Adapter Type '{}' does not exist!", type);
		return nullptr;
	}

	if (!output->ParseSettings(project["Output"]))
	{
		Error("Output Adapter Parse settings failed!");
		return nullptr;
	}

	return output;
}
