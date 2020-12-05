#pragma once

#include "Core/Scene/AccelerationStructure.h"

class None : public AccelerationStructure
{
public:
	virtual bool Parse(const YAML::Node& node) override;

	virtual bool Intersect(const Ray& ray, RayIntersection& intersection) override;
	virtual bool TestIntersect(const Ray& ray) override;

	virtual void Build(const Scene& scene) override;

private:
	const std::vector<Object>* m_Objects;
};
