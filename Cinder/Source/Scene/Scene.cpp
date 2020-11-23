#include "Scene.h"

Scene Scene::FromFile(std::string file)
{
	YAML::Node node = YAML::LoadFile(file);
	return node.as<Scene>();
}

void Scene::Setup()
{
	// Link material pointers to the actual material
	for (auto object : m_Objects)
	{
		object->SetMaterialPointer(&m_Materials[object->m_MaterialName]);
	}
}

namespace YAML
{
bool convert<Scene>::decode(const Node& node, Scene& scene)
{
	// Load Render Settings
	scene.m_Settings = node["Settings"].as<RenderSettings>();

	// Load Camera
	std::string camera = "Main Camera";
	if (!scene.m_Settings.CameraName.empty()) { camera = scene.m_Settings.CameraName; }
	bool found = false;
	for (auto& map : node["Cameras"])
	{
		if (map[camera]) 
		{ 
			scene.m_Camera = map[camera].as<Camera>();
			found = true;
			break;
		}
	}
	if (!found) { Error("Camera '" COLOR, camera, "\x1b[31m' does not exist!"); }

	// Load Objects
	for (auto& map : node["Objects"])
	{
		auto object = *map.begin();
		scene.m_Objects.emplace_back(object.second.as<Object*>());
	}

	// Load Materials
	for (auto& map : node["Materials"])
	{
		auto material = *map.begin();
		scene.m_Materials.emplace(material.first.as<std::string>(), material.second.as<Material>());
	}

	scene.Setup();

	return true;
}
}
