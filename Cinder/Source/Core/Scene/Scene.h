#pragma once

#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"
#include "Core/Scene/Object.h"
#include "Core/Scene/Environment.h"
#include "Core/Material/Material.h"
#include "Core/Scene/Light.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	~Scene();
	static Scene* Load(const std::string& file);

	Camera& GetCamera() const;
	const std::vector<Object>& GetObjects() const { return m_Objects; }
	const std::vector<up<Light>>& GetLights() const { return m_Lights; }
	
	const Bound& GetBound() const { return m_Bound; }

	bool Intersect(const Ray& ray, Interaction& interaction) const;
	bool TestIntersect(const Ray& ray) const;

	const Environment& GetEnvironment() const { return m_Environment; }

private:
	friend struct YAML::convert<Scene*>;
	friend void RunProject(const std::filesystem::path& filePath);

	void SetCameraAspectRatio(float aspectRatio);
	bool LinkReferences();

	up<AccelerationStructure> m_Acceleration;
	up<Camera> m_Camera = nullptr;
	std::vector<up<Geometry>> m_Geometry;
	std::vector<up<Material>> m_Materials;
	std::vector<up<Light>> m_Lights;
	std::vector<Object> m_Objects;

	Environment m_Environment;
	
	Bound m_Bound = Bound({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
