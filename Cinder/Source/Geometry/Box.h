#pragma once

#include "Core/Scene/Geometry.h"

class Box : public Geometry
{
public:
	Box(const std::string& name);

	virtual bool Parse(const YAML::Node& node) override;

	virtual const Bound& GetBound() const override { return m_Bound; }

	virtual bool Intersect(const Ray& ray, Interaction& interaction) const override;
	virtual bool TestIntersect(const Ray& ray) const override;

	virtual float GetArea() const override;

	virtual const std::vector<Geometry*>& GetSubGeometry() override { return m_Sub; }

private:
	Bound m_Bound;
	std::vector<Geometry*> m_Sub;
};
