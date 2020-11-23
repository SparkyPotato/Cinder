#pragma once

#include "Vector.h"

class Ray
{
public:
	Ray(const Vector& origin, const Vector& direction);

	const Vector& GetOrigin() const { return m_Origin; }
	const Vector& GetDirection() const { return m_Direction; }

private:
	Vector m_Origin;
	Vector m_Direction;
};
