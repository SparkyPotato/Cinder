#pragma once

#include "Core/Scene/Geometry.h"

class Sphere : public Geometry
{
public:
	Sphere(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) override;
	virtual bool TestIntersect(const Ray& ray) override;

private:
	float m_Radius = 0.f;
};
