#pragma once

#include "Core/Scene/AccelerationStructure.h"

class ASNone : public AccelerationStructure
{
public:
	void Build(const Scene& scene) override;

	bool Intersect(const Ray& ray, RayIntersection& intersection) override;
	bool TestIntersect(const Ray& ray) override;

private:
	const Scene* m_Scene = nullptr;
};
