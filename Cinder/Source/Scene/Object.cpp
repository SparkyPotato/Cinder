#include "Object.h"

#include "Objects/Sphere.h"

Object::Object(const Transform& transform, const std::string& material)
	: m_Transform(transform), m_MaterialName(material)
{}

void Object::SetMaterialPointer(Material* pointer)
{
	m_Material = pointer;
}

namespace YAML
{
bool convert<Object*>::decode(const Node& node, Object*& object)
{
	if (!node["Type"])
	{
		Error("Object does not have a type! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Transform"])
	{
		Error("Object does not have a transform! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Material"])
	{
		Error("Object does not have a material! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	auto type = node["Type"].as<std::string>();

	if (type == "Sphere")
	{
		if (!node["Radius"])
		{
			Error("Sphere does not have a radius! ("
				COLOR, "line ", node.Mark().line, "\x1b[31m).");
		}

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
