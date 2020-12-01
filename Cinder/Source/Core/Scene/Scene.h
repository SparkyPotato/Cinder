#pragma once

#include "Camera.h"
#include "Object.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	~Scene();
	
	static Scene* FromFile(const std::string& file);

	std::vector<Object> Objects;
	std::vector<Material> Materials;
	AccelerationStructure* AccelStructure;
	Camera* MainCamera;
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
