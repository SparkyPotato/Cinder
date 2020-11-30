#pragma once
#include "Core/Scene/Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera() = default;
	
	bool ParseProperties(const YAML::Node& node) override;
	
private:
	float m_FOV;
};
