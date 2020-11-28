#include "PCH.h"
#include "Run.h"

#include "Core/Components/Renderer.h"
#include "Core/Components/Framebuffer.h"
#include "Core/Components/OutputAdapter.h"

void RunProject(const std::filesystem::path& filePath)
{
	YAML::Node project;
	try { project = YAML::LoadFile(filePath.string()); }
	catch (...)
	{
		Error("Failed to parse project file! Skipping.");
		return;
	}
	
	std::string type = "Preview";
	if (project["Renderer"]["Type"]) { type = project["Renderer"]["Type"].as<std::string>(); }
	else { Warning("No Renderer Type, using default '{}'.", type); }

	Renderer* renderer;
	try { renderer = ComponentManager::Get()->SpawnRenderer(type); }
	catch (...) { Error("Renderer Type '{}' does not exist! Skipping.", type); return; }
	if (!renderer->ParseSettings(project["Renderer"])) { Error("Parse settings failed! Skipping."); return;  }

	type = "Simple";
	uint32_t width = 1920, height = 1080;
	if (project["Framebuffer"]["Type"]) { type = project["Framebuffer"]["Type"].as<std::string>(); }
	else { Warning("No Framebuffer Type, using default ({}).", type); }
	if (project["Framebuffer"]["Width"]) { width = project["Framebuffer"]["Width"].as<uint32_t>(); }
	else { Warning("No Framebuffer Width, using default ({}).", width); }
	if (project["Framebuffer"]["Height"]) { height = project["Framebuffer"]["Height"].as<uint32_t>(); }
	else { Warning("No Framebuffer Height, using default ({}).", width); }

	Framebuffer* framebuffer;
	try { framebuffer = ComponentManager::Get()->SpawnFramebuffer(type, width, height); }
	catch (...) { Error("Framebuffer Type '{}' does not exist! Skipping.", type); return; }
	if (!framebuffer->ParseSettings(project["Framebuffer"])) { Error("Parse settings failed! Skipping."); return; }

	type = "PNG";
	std::string file = "Output.png";
	if (project["Output"]["Type"]) { type = project["Output"]["Type"].as<std::string>(); }
	else { Warning("No Output Type, using default ({})", type); }
	if (project["Output"]["File"]) { file = project["Output"]["File"].as<std::string>(); }
	else { Warning("No Output File, using default ({})", file); }

	OutputAdapter* output;
	std::string outputPath = filePath.parent_path().string() + "/" + file;
	try { output = ComponentManager::Get()->SpawnOutputAdapter(type, outputPath); }
	catch (...) { Error("Output Adapter Type '{}' does not exist! Skipping.", type); return; }
	if (!output->ParseSettings(project["Output"])) { Error("Parse settings failed! Skipping."); return; }

	output->WriteOutput(*framebuffer);
}
