#pragma once

#include "Globals.h"
#include "Math/Transform.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Transform& transform, float verticalFOV);

private:
	Transform m_Transform;
	float m_FOV;
};

namespace YAML
{
template<>
struct convert<Camera>
{
	static bool decode(const Node& node, Camera& camera);
};
}
