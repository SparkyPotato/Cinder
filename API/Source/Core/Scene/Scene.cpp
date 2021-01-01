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

#include "PCH.h"
#include "Scene.h"

#include "Core/Scene/AccelerationStructure.h"

#include "Lights/Area.h"

Scene* Scene::Load(const std::string& file)
{
	YAML::Node node = YAML::LoadFile(file);
	return node.as<Scene*>();
}

Scene::~Scene() = default;

Camera& Scene::GetCamera() const
{
	return *m_Camera;
}

bool Scene::Intersect(const Ray& ray, Interaction& interaction) const
{
	return m_Acceleration->Intersect(ray, interaction);
}

bool Scene::TestIntersect(const Ray& ray) const
{
	return m_Acceleration->TestIntersect(ray);
}

void Scene::SetCameraAspectRatio(float aspectRatio)
{
	m_Camera->SetAspectRatio(aspectRatio);
}

bool Scene::LinkReferences()
{
	for (auto& object : m_Objects)
	{
		// Setup object to world transform to object to camera
		object.ToCamera = object.ToCamera * m_Camera->ToWorld.GetInverse();
		//                   object to world         world to camera

		// Link geometry pointers to the actual geometry
		for (auto& geometry : m_Geometry)
		{
			if (geometry->Name == object.m_GeometryName)
			{
				object.m_Geometry = geometry.get();
				break;
			}
		}
		if (!object.m_Geometry) { Error("Geometry '{}' does not exist!", object.m_GeometryName); return false; }

		for (auto& material : m_Materials)
		{
			if (material->Name == object.m_MaterialName)
			{
				object.m_Material = material.get();
				if (material->GetEmission())
				{
					m_Lights.push_back(std::make_unique<AreaLight>(
						1,
						material->GetEmission(),
						material->GetEmissionIntensity(),
						&object
					));
				}
				break;
			}
		}
		if (!object.m_Material) { Error("Material '{}' does not exist!", object.m_MaterialName); return false; }
		
		m_Bound = Union(m_Bound, object.GetBound());
	}

	for (auto& light : m_Lights)
	{
		light->ToCamera = light->ToCamera * m_Camera->ToWorld.GetInverse();
		light->Preprocess(*this);
	}

	m_Acceleration->Build(*this);

	return true;
}

bool YAML::convert<Scene*>::decode(const Node& node, Scene*& scene)
{
	scene = new Scene;

	try { scene->m_Camera = node["Camera"].as<up<Camera>>(); }
	catch (...) { return false; }

	if (!node["Geometry"] || !node["Geometry"].IsSequence())
	{
		Error("Geometry list must be a sequence (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Objects"] || !node["Objects"].IsSequence())
	{
		Error("Object list must be a sequence (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Materials"] || !node["Materials"].IsSequence())
	{
		Error("Material list must be a sequence (line {})!", node.Mark().line + 1);
		return false;
	}
	if (node["Lights"])
	{
		if (!node["Lights"].IsSequence())
		{
			Error("Light list must be a sequence (line {})!", node.Mark().line + 1);
			return false;
		}

		for (auto& light : node["Lights"])
		{
			scene->m_Lights.emplace_back(light.as<up<Light>>());
		}
	}

	if (!node["Acceleration"])
	{
		Error("No Acceleration Structure present (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Acceleration"]["Type"])
	{
		Error("No Acceleration Type (line {})!", node["Acceleration"].Mark().line + 1);
		return false;
	}
	std::string aType;
	try { aType = node["Acceleration"]["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Acceleration type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	try { scene->m_Acceleration = Registry::Get()->GAcceleration.at(aType)(); }
	catch (...)
	{
		Error("Acceleration type '{}' does not exist (line {})!", aType, node["Acceleration"]["Type"].Mark().line + 1);
		return false;
	}
	if (!scene->m_Acceleration->Parse(node["Acceleration"])) { return false; }

	for (auto& geometry : node["Geometry"])
	{
		scene->m_Geometry.emplace_back(geometry.as<up<Geometry>>());
	}

	for (auto& material : node["Materials"])
	{
		scene->m_Materials.emplace_back(material.as<up<Material>>());
	}
	
	for (auto& object : node["Objects"])
	{
		scene->m_Objects.emplace_back(object.as<Object>());
	}

	return scene->LinkReferences();
}

bool Occlusion::operator()(const Scene& scene)
{
	return scene.TestIntersect(Point1.SpawnRayTo(Point2));
}
