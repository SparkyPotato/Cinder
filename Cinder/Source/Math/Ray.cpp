#include "Ray.h"

Ray::Ray(const Vector& origin, const Vector& direction)
	: m_Origin(origin), m_Direction(direction.GetNormalized())
{}
