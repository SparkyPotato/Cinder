#pragma once

#include "Core/Components/Camera.h"
#include "Object.h"
#include "Cubemap.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	
	static Scene* FromFile(const std::string& file);

	std::vector<Object> Objects;
	std::vector<Material> Materials;
	Cubemap Skybox;

	AccelerationStructure* AccelStructure;
	Camera* MainCamera;
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
