#pragma once

#include "Core/Components/ComponentManager.h"
#include "RayIntersection.h"
#include "Transform.h"

class Shape
{
public:
	Shape(const Transform& objectToWorld)
		: m_ObjectToWorld(objectToWorld)
	{}
	virtual ~Shape() {}

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) = 0;
	virtual bool TestIntersect(const Ray& ray) 
	{ 
		RayIntersection discard;
		return Intersect(ray, discard);
	}

	virtual bool ParseProperties(const YAML::Node& node) = 0;

protected:
	Transform m_ObjectToWorld;
};

template<>
struct YAML::convert<Shape*>
{
	static bool decode(const Node& node, Shape*& shape)
	{
		if (!node["Type"])
		{
			Error("Shape has no Type!");
			return false;
		}
		if (!node["Transform"])
		{
			Error("Shape has no transform!");
			return false;
		}

		std::string type;
		try { type = node["Type"].as<std::string>(); }
		catch (YAML::Exception& e)
		{
			Error("Shape Type must be a string (line {})!", e.mark.line);
			return false;
		}

		Transform transform;
		try { transform = node["Transform"].as<Transform>(); }
		catch (...)
		{
			Error("Failed to parse transform!");
			return false;
		}

		try { shape = ComponentManager::Get()->SpawnShape(type, transform); }
		catch (...)
		{
			Error("Shape Type '{}' does not exist!", type);
			return false;
		}
		if (!shape->ParseProperties(node))
		{
			Error("Shape Parse Properties failed!");
			return false;
		}

		return true;
	}
};

#define REGISTER_SHAPE(name, className) \
Shape* Spawn##className(const Transform& transform) { return new className(transform); } \
struct Register##className \
{ \
	Register##className() \
	{ \
		ComponentManager::Get()->RegisterShape(#name, &Spawn##className); \
	} \
}; \
static Register##className StaticRegister##className;
