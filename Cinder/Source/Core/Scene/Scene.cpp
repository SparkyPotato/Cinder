#include "PCH.h"
#include "Scene.h"

#include "Core/Scene/AccelerationStructure.h"

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

bool Scene::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	return m_Acceleration->Intersect(ray, intersection);
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
		object.m_ToCamera = object.m_ToCamera * m_Camera->ToWorld.GetInverse();
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
				break;
			}
		}
		if (!object.m_Material) { Error("Material '{}' does not exist!", object.m_MaterialName); return false; }
	}

	m_Environment.m_CameraToWorld = m_Camera->ToWorld;
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
	
	if (!node["Environment"])
	{
		Error("Scene does not have environment!");
		return false;
	}
	try { scene->m_Environment = node["Environment"].as<Environment>(); }
	catch (...) { return false; }

	return scene->LinkReferences();
}
