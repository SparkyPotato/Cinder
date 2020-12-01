#pragma once
#include "Core/Components/Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera() = default;
	
	bool ParseProperties(const YAML::Node& node) override;

	void SetAspectRatio(float aspectRatio) override;
	Ray GetRay(float x, float y) override;
	
private:
	float m_FOV = 0.f;

	float m_AspectX = 0.f, m_AspectY = 0.f;
};
