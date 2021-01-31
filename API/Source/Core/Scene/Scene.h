//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"
#include "Core/Scene/Object.h"
#include "Core/Scene/Light.h"
#include "Core/Material/Material.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	~Scene();

	/// Load a scene from a file.
	///
	/// \param file The path of the scene description file.
	/// 
	/// \return The loaded scene.
	static Scene* Load(const std::string& file);

	/// Get the scene camera.
	///
	/// \return The camera.
	Camera& GetCamera() const;

	/// Get the objects in the scene.
	///
	/// \return std::vector of objects.
	const std::vector<Object>& GetObjects() const { return m_Objects; }
	
	/// Get the bound of the scene.
	///
	/// \return The bound of the scene in camera space.
	const Bound& GetBound() const { return m_Bound; }

	/// Intersect a ray with the scene.
	///
	/// \param ray The ray to intersect.
	/// \param interaction The interaction to fill in (in camera space).
	/// 
	/// \return If there was a hit.
	bool Intersect(const Ray& ray, Interaction& interaction) const;

	/// Test intersect a ray with the scene.
	///
	/// \param ray The ray to intersect.
	/// 
	/// \return If there was a hit.
	bool TestIntersect(const Ray& ray) const;
	
	/// Get the lights in the scene.
	///
	/// \return The lights in scene.
	const std::vector<up<Light>>& GetLights() const { return m_Lights; }

private:
	friend struct YAML::convert<Scene*>;
	friend bool RunCinderProject(const char* filePath, float* timeSeconds);

	void SetCameraAspectRatio(float aspectRatio);
	bool LinkReferences();

	up<AccelerationStructure> m_Acceleration;
	up<Camera> m_Camera = nullptr;
	std::vector<up<Geometry>> m_Geometry;
	std::vector<up<Material>> m_Materials;
	std::vector<up<Light>> m_Lights;
	std::vector<Object> m_Objects;
	
	Bound m_Bound = Bound({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
