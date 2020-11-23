#pragma once

#undef max

#include "Vector.h"

class Object;

class Ray
{
public:
	Ray(const Vector& origin, const Vector& direction);

	const Vector& GetOrigin() const { return m_Origin; }
	const Vector& GetDirection() const { return m_Direction; }

	void SetIntersection(Object* object, float distance);
	Vector GetIntersectionPoint() const;
	const Object* GetObject() const;

private:
	Vector m_Origin;
	Vector m_Direction;

	Object* m_HitObject = nullptr;
	float m_IntersectionDistance =
		std::numeric_limits<float>::max();
};
