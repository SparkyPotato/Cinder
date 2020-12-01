#pragma once

#include "Core/Math/Vector.h"

class Object;

class RayIntersection
{
public:
	Point HitPoint;
	Normal HitNormal;
	const Object* HitObject  = nullptr;

	float U = 0.f, V = 0.f;
};
