#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Ray.h"

class Object;

constexpr float ShadowEpsilon = 0.001f;

class RayIntersection
{
public:
	Point HitPoint;
	Normal HitNormal;
	const Object* HitObject  = nullptr;

	float U = 0.f, V = 0.f;

	Ray SpawnRayTo(const RayIntersection& other)
	{
		Point origin = HitPoint + Vector(HitNormal) * ShadowEpsilon;
		Vector direction = other.HitPoint - origin;

		return Ray(origin, direction);
	}
};
