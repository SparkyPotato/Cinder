//    Copyright 2021 SparkyPotato
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
#include "Core/Scene/Environment.h"
#include "Core/Material/Material.h"

class AccelerationStructure;

class Scene
{
public:
	Scene() = default;
	~Scene();
	static Scene* Load(const std::string& file);

	Camera& GetCamera() const;
	const std::vector<Object>& GetObjects() const { return m_Objects; }
	
	const Bound& GetBound() const { return m_Bound; }

	bool Intersect(const Ray& ray, Interaction& interaction) const;
	bool TestIntersect(const Ray& ray) const;

	const Environment& GetEnvironment() const { return m_Environment; }
	const std::vector<Emission*>& GetEmission() const { return m_Emission; }

private:
	friend struct YAML::convert<Scene*>;
	friend bool RunCinderProject(const char* filePath, float* timeSeconds);

	void SetCameraAspectRatio(float aspectRatio);
	bool LinkReferences();

	up<AccelerationStructure> m_Acceleration;
	up<Camera> m_Camera = nullptr;
	std::vector<up<Geometry>> m_Geometry;
	std::vector<up<Material>> m_Materials;
	std::vector<Object> m_Objects;
	std::vector<Emission*> m_Emission;

	Environment m_Environment;
	
	Bound m_Bound = Bound({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
};

template<>
struct YAML::convert<Scene*>
{
	static bool decode(const Node& node, Scene*& scene);
};
