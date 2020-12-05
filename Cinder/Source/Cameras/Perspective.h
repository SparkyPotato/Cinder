#pragma once

#include "Core/Scene/Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const Transform& transform);

	virtual bool Parse(const YAML::Node& node) override;

	virtual Ray GetRay(float u, float v) override;

private:
	float m_FOV;
};
