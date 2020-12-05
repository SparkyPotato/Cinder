#include "PCH.h"
#include "Scene.h"

Scene* Scene::Load(const std::string& file)
{
	YAML::Node node = YAML::LoadFile(file);
	return node.as<Scene*>();
}

Scene::~Scene()
{
	for (auto geometry : m_Geometry)
	{
		delete geometry;
	}

	delete m_Camera;
}

Camera& Scene::GetCamera() const
{
	return *m_Camera;
}

void Scene::SetCameraAspectRatio(float aspectRatio)
{
	m_Camera->SetAspectRatio(aspectRatio);
}

void Scene::LinkReferences()
{
	for (auto& object : m_Objects)
	{
		// Setup object to world transform to object to camera
		object.m_ToCamera = object.m_ToCamera * m_Camera->ToWorld.GetInverse();
		//                   object to world         world to camera
		
		// Link geometry pointers to the actual geometry
		for (auto geometry : m_Geometry)
		{
			if (geometry->Name == object.m_GeometryName)
			{
				object.m_Geometry = geometry;
				break;
			}
		}
	}
}

bool YAML::convert<Scene*>::decode(const Node& node, Scene*& scene)
{
	scene = new Scene;

	try { scene->m_Camera = node["Camera"].as<Camera*>(); }
	catch (...) { return false; }

	if (!node["Geometry"].IsSequence())
	{
		Error("Geometry list must be a sequence (line {})!", node["Geometry"].Mark().line + 1);
		return false;
	}

	for (auto& geometry : node["Geometry"])
	{
		scene->m_Geometry.emplace_back(geometry.as<Geometry*>());
	}
	
	for (auto& object : node["Objects"])
	{
		scene->m_Objects.emplace_back(object.as<Object>());
	}
	
	scene->LinkReferences();

	return true;
}
