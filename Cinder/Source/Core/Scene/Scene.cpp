#include "PCH.h"
#include "Scene.h"

Scene* Scene::Load(const std::string& file)
{
	YAML::Node node = YAML::LoadFile(file);
	return node.as<Scene*>();
}

Camera& Scene::GetCamera() const
{
	return *m_Camera;
}

void Scene::SetCameraAspectRatio(float aspectRatio)
{
	m_Camera->SetAspectRatio(aspectRatio);
}

bool YAML::convert<Scene*>::decode(const Node& node, Scene*& scene)
{
	scene = new Scene;

	try { scene->m_Camera = node["Camera"].as<Camera*>(); }
	catch (...) { return false; }

	if (!node["Geometry"].IsSequence())
	{
		Error("Geometry list must be a sequence (line {})!", node["Geometry"].Mark().line + 1);
		return false;
	}

	for (auto& geometry : node["Geometry"])
	{
		scene->m_Geometry.emplace_back(geometry.as<Geometry*>());
	}

	return true;
}
