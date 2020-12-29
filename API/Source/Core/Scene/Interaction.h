//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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
