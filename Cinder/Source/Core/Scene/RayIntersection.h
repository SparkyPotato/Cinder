#pragma once

#include "Core/Math/Vector.h"

class Object;

class RayIntersection
{
public:
	Point HitPoint;
	Normal HitNormal;
	const Object* HitObject;
};
