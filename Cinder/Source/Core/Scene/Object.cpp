#include "PCH.h"
#include "Object.h"

#include "Core/Components/ComponentManager.h"

bool YAML::convert<Object>::decode(const Node& node, Object& object)
{
	if (!node["Shape"])
	{
		Error("Object has no shape!");
		return false;
	}
	try { object.ObjectShape = node["Shape"].as<Shape*>(); }
	catch (...)
	{
		Error("Shape creation failed!");
		return false;
	}

	return true;
}

bool Object::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	bool ret = ObjectShape->Intersect(ray, intersection);
	if (ret) { intersection.HitObject = this; }
	return ret;
}

bool Object::TestIntersect(const Ray& ray) const
{
	return ObjectShape->TestIntersect(ray);
}
