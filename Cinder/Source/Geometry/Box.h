#pragma once

#include "Core/Scene/Geometry.h"

class Box : public Geometry
{
public:
	Box(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual Bound GetBound() const override;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

private:
	Bound m_Bound;
};
