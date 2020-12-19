#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Ray.h"

class Object;
class BSDF;

constexpr float Epsilon = 0.002f;

class Interaction
{
public:
	Point HitPoint;
	Normal GNormal;
	const Object* HitObject  = nullptr;
	BSDF* Bsdf = nullptr;

	Normal SNormal;
	Vector Tangent, Bitangent;
	float U = 0.f, V = 0.f;

	Ray SpawnRayTo(const Interaction& other)
	{
		Point origin = HitPoint + Vector(GNormal) * Epsilon;
		Vector direction = other.HitPoint - origin;
		float extent = direction.GetLength() - Epsilon;

		return Ray(origin, direction, extent);
	}
};
