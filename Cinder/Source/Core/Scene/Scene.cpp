#include "PCH.h"
#include "Scene.h"

Transform GCameraToWorld;

Scene* Scene::FromFile(const std::string& file)
{
	try
	{
		YAML::Node scene = YAML::LoadFile(file);
		return scene.as<Scene*>();
	}
	catch (YAML::Exception& e)
	{
		Error("Failed to parse scene (line {})!", e.mark.line + 1);
		throw e;
	}
}

bool YAML::convert<Scene*>::decode(const Node& node, Scene*& scene)
{
	scene = Memory::Get()->Allocate<Scene>();
	
	if (!node["Camera"])
	{
		Error("No Camera (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Camera"]["Transform"])
	{
		Error("Camera does not have transform (line {})!", node["Camera"].Mark().line + 1);
		return false;
	}
	if (!node["Camera"]["Type"])
	{
		Error("Camera does not have a type (line {})!", node["Camera"].Mark().line + 1);
		return false;
	}
	
	GCameraToWorld = node["Camera"]["Transform"].as<Transform>();
	std::string type;
	try { type = node["Camera"]["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	try { scene->MainCamera = ComponentManager::Get()->SpawnCamera(type); }
	catch (...)
	{
		Error("Camera type '{}' does not exist!", type);
		return false;
	}
	if (!scene->MainCamera->ParseProperties(node["Camera"]))
	{
		Error("Failed Camera Parse Properties!");
		return false;
	}
	
	if (!node["Objects"])
	{
		Error("No Object list present (line {})!", node.Mark().line + 1);
		return false;
	}

	if (!node["Objects"].IsSequence())
	{
		Error("Object list must be a sequence (line {})!", node["Objects"].Mark().line + 1);
		return false;
	}
	
	if (!node["Materials"])
	{
		Error("No Material list present (line {})!", node.Mark().line + 1);
		return false;
	}
	
	if (!node["Materials"].IsSequence())
	{
		Error("Material list must be a sequence (line {})!", node["Materials"].Mark().line + 1);
		return false;
	}

	for (auto& object : node["Objects"])
	{
		scene->Objects.emplace_back(object.as<Object>());
	}
	
	for (auto& material : node["Materials"])
	{
		scene->Materials.emplace_back(material.as<Material>());
	}
	
	for (auto& object : scene->Objects)
	{
		for (auto& material : scene->Materials)
		{
			if (object.MaterialName == material.Name)
			{
				object.ObjectMaterial = &material;
				break;
			}
		}
		
		if (!object.ObjectMaterial)
		{
			Error("Material '{}' does not exist!", object.MaterialName);
			return false;
		}
	}

	if (!node["Skybox"])
	{
		Warning("No skybox present, using default black.");
		scene->Skybox = DefaultSkybox();
	}
	else
	{
		scene->Skybox = node["Skybox"].as<Cubemap>();
	}

	return true;
}
