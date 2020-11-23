#pragma once

#include "Globals.h"
#include "Math/Transform.h"

class Material;

class Object
{
public:
	Object(const Transform& transform, const std::string& material);
	virtual ~Object() = default;

private:
	friend class Scene;
	void SetMaterialPointer(Material* pointer);

	Transform m_Transform;
	std::string m_MaterialName;
	Material* m_Material;
};

class Sphere : public Object
{
public:
	Sphere(const Transform& transform, const std::string& material, float radius);

private:
	float m_Radius;
};

namespace YAML
{
template<>
struct convert<Object*>
{
	static bool decode(const Node& node, Object*& object);
};
}
