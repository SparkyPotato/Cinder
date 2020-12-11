#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Ray.h"

class Object;
class BSDF;

constexpr float ShadowEpsilon = 0.001f;

class Interaction
{
public:
	Point HitPoint;
	Normal GNormal;
	Normal SNormal;
	const Object* HitObject  = nullptr;
	BSDF* Bsdf = nullptr;

	float U = 0.f, V = 0.f;

	Ray SpawnRayTo(const Interaction& other)
	{
		Point origin = HitPoint + Vector(GNormal) * ShadowEpsilon;
		Vector direction = other.HitPoint - origin;

		return Ray(origin, direction);
	}
};
