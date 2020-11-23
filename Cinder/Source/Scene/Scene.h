#pragma once

#include "Camera.h"
#include "Globals.h"
#include "Object.h"
#include "Material.h"
#include "RenderSettings.h"

class Scene
{
public:
	Scene() = default;

	static Scene FromFile(std::string file);

private:
	friend YAML::convert<Scene>;
	void Setup();

	RenderSettings m_Settings;
	Camera m_Camera;
	std::vector<Object*> m_Objects;
	std::map<std::string, Material> m_Materials;
};

namespace YAML
{
template<>
struct convert<Scene>
{
	static bool decode(const Node& node, Scene& scene);
};
}
