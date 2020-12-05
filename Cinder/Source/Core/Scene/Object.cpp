#include "PCH.h"
#include "Core/Scene/Object.h"

bool Object::Intersect(const Ray& ray, RayIntersection& intersection) const
{
	bool hit = m_Geometry->Intersect(m_ToCamera.GetInverse()(ray), intersection);
	if (hit)
	{
		intersection.HitObject = this;
		intersection.HitPoint = m_ToCamera(intersection.HitPoint);
		intersection.HitNormal = m_ToCamera(intersection.HitNormal);
	}
	
	return true;
}

bool Object::TestIntersect(const Ray& ray) const
{
	return m_Geometry->TestIntersect(m_ToCamera.GetInverse()(ray));
}

bool YAML::convert<Object>::decode(const Node& node, Object& object)
{
	if (!node["Transform"])
	{
		Error("Object does not have transform (line {})!", node.Mark().line + 1);
		return false;
	}
	if (!node["Geometry"])
	{
		Error("Object does not have geometry (line {})!", node.Mark().line + 1);
		return false;
	}
	
	// Just store object to world, scene will later set it to object to camera
	object.m_ToCamera = node["Transform"].as<Transform>();
	
	try { object.m_GeometryName = node["Geometry"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Geometry reference must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	return true;
}
