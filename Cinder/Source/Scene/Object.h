#pragma once

#include "Globals.h"
#include "Math/Transform.h"
#include "Math/Ray.h"

class Material;

class Object
{
public:
	Object(const Transform& transform, const std::string& material);
	virtual ~Object() = default;

	// Calculate location of intersection
	virtual bool Intersect(Ray& worldRay) = 0;

	// Does not set the intersection point in the ray,
	// so is faster
	virtual bool TestIntersect(const Ray& worldRay) = 0;

	const Material* GetMaterial() const { return m_Material; }

protected:
	friend class Scene;
	void SetMaterialPointer(Material* pointer);

	Transform m_Transform;
	std::string m_MaterialName;
	Material* m_Material = nullptr;
};

namespace YAML
{
template<>
struct convert<Object*>
{
	static bool decode(const Node& node, Object*& object);
};
}
