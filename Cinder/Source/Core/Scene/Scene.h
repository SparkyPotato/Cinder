#pragma once

#include "Core/Components/Camera.h"
#include "Object.h"
#include "Skybox.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	~Scene();
	
	static Scene* FromFile(const std::string& file);

	std::vector<Object> Objects;
	std::vector<Material> Materials;
	Skybox SceneSkybox;

	AccelerationStructure* AccelStructure;
	Camera* MainCamera;
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
