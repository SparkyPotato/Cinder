#pragma once

#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"

class Scene
{
public:
	Scene() = default;
	static Scene* Load(const std::string& file);

	Camera& GetCamera() const;

private:
	friend struct YAML::convert<Scene*>;
	friend void RunProject(const std::filesystem::path& filePath);

	void SetCameraAspectRatio(float aspectRatio);

	Camera* m_Camera;
	std::vector<Geometry*> m_Geometry;
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
