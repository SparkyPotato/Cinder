#pragma once

#include "Core/Scene/Shape.h"

class Sphere : public Shape
{
public:
	Sphere(const Transform& objectToCamera);

	bool Intersect(const Ray& ray, RayIntersection& intersection) override;
	bool TestIntersect(const Ray& ray) override;

	bool ParseProperties(const YAML::Node& node) override;

private:
	float m_Radius = 0.f;
};
