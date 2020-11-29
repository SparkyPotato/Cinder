#include "PCH.h"
#include "Scene.h"

Scene Scene::FromFile(const std::string& file)
{
	YAML::Node scene = YAML::LoadFile(file);
	return scene.as<Scene>();
}


bool YAML::convert<Scene>::decode(const Node& node, Scene& scene)
{
	if (!node["Objects"])
	{
		Error("No Object list present!");
		return false;
	}

	if (!node["Objects"].IsSequence())
	{
		Error("Object list must be a sequence!");
		return false;
	}

	for (auto object : node["Objects"])
	{
		scene.Objects.emplace_back(object.as<Object>());
	}

	return true;
}
