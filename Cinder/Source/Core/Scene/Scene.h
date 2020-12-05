#pragma once

#include "Core/Scene/AccelerationStructure.h"
#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"
#include "Core/Scene/Object.h"

class Scene
{
public:
	Scene() = default;
	~Scene();
	static Scene* Load(const std::string& file);

	Camera& GetCamera() const;
	const std::vector<Object>& GetObjects() const { return m_Objects; }

	bool Intersect(const Ray& ray, RayIntersection& intersection);
	bool TestIntersect(const Ray& ray);

private:
	friend struct YAML::convert<Scene*>;
	friend void RunProject(const std::filesystem::path& filePath);

	void SetCameraAspectRatio(float aspectRatio);
	void LinkReferences();

	AccelerationStructure* m_Acceleration;
	Camera* m_Camera = nullptr;
	std::vector<Geometry*> m_Geometry;
	std::vector<Object> m_Objects;
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
