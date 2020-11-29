#pragma once

#include "Object.h"

class Scene
{
public:
	Scene() = default;
	static Scene FromFile(const std::string& file);

	std::vector<Object> Objects;
};

template<>
struct YAML::convert<Scene>
{
	static bool decode(const Node& node, Scene& scene);
};
