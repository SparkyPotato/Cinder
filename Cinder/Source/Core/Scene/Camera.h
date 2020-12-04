#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Ray.h"
#include "Core/Math/Transform.h"

class Camera
{
public:
	Camera(const Transform& toWorld)
		: ToWorld(toWorld)
	{}
	virtual ~Camera() {}

	void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	virtual bool Parse(const YAML::Node& node) = 0;

	virtual Ray GetRay(float u, float v) = 0;

	Transform ToWorld;

protected:
	float m_AspectRatio = 0.f;
};

template<>
struct YAML::convert<Camera*>
{
	static bool decode(const Node& node, Camera*&);
};

#define CAMERA(type, className) \
Camera* Spawn##className(const Transform& t) { return Memory::Get()->Allocate<className>(t); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GCameras.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
