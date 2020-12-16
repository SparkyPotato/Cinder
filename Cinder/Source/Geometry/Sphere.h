#pragma once

#include "Core/Scene/Geometry.h"

class Sphere : public Geometry
{
public:
	Sphere(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual Bound GetBound() const override;

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual float GetArea() const override;

	virtual const std::vector<Geometry*>& GetSubGeometry() override { return m_Sub; }

private:
	float m_Radius = 0.f;
	std::vector<Geometry*> m_Sub;
};
