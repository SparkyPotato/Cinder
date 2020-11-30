#pragma once
#include "Core/Scene/Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera() = default;
	
	bool ParseProperties(const YAML::Node& node) override;

	void RegisterFramebuffer(const Framebuffer& framebuffer) override;
	Ray GetRay(uint32_t x, uint32_t y) override;
	
private:
	float m_FOV = 0.f;

	float m_AspectX = 0.f, m_AspectY = 0.f;
	const Framebuffer* m_Framebuffer = nullptr;
};
