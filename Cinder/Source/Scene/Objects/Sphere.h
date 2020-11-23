#pragma once

#include "Scene/Object.h"

class Sphere : public Object
{
public:
	Sphere(const Transform& transform, const std::string& material, float radius);

	bool Intersect(Ray& worldRay) override;

	bool TestIntersect(const Ray& worldRay) override;

private:
	float m_RadiusSquare;
};
