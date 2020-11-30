#include "PCH.h"
#include "Scene.h"

Transform GCameraTransform;

Scene::~Scene()
{
	for (auto& object : Objects)
	{
		if (object.ObjectShape) { delete object.ObjectShape; }
	}
}

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
	scene = new Scene;
	
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
	
	GCameraTransform = node["Camera"]["Transform"].as<Transform>();
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

	for (auto object : node["Objects"])
	{
		scene->Objects.emplace_back(object.as<Object>());
	}

	return true;
}