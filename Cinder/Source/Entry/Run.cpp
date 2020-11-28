#include "PCH.h"
#include "Run.h"

#include "yaml-cpp/yaml.h"

void RunProject(const std::filesystem::path& filePath)
{
	YAML::Node project;
	try { project = YAML::LoadFile(filePath.string()); }
	catch (...)
	{
		Error("Failed to parse project file! Skipping.");
		return;
	}
	
	
}
