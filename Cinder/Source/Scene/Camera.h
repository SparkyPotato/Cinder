#pragma once

#include "Globals.h"
#include "Math/Transform.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Transform& transform, float verticalFOV);

	const Matrix& GetCameraToWorldMatrix() const { return m_Transform.GetTransform(); }
	float GetFOV() const { return m_FOV; }

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
