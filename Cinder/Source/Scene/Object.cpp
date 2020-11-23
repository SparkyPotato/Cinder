#include "Object.h"

Object::Object(const Transform& transform, const std::string& material)
	: m_Transform(transform), m_MaterialName(material)
{}

void Object::SetMaterialPointer(Material* pointer)
{
	m_Material = pointer;
}

Sphere::Sphere(const Transform& transform, const std::string& material, float radius)
	: Object(transform, material), m_Radius(radius)
{}

namespace YAML
{
bool convert<Object*>::decode(const Node& node, Object*& object)
{
	auto type = node["Type"].as<std::string>();

	if (type == "Sphere")
	{
		object = new Sphere(
			node["Transform"].as<Transform>(),
			node["Material"].as<std::string>(),
			node["Radius"].as <float>()
		);
	}
	else
	{
		Error("Type '" COLOR, type.c_str(), "\x1b[31m' does not exist! (",
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	return true;
}
}
