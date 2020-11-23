#include "Ray.h"

#include "Globals.h"

Ray::Ray(const Vector& origin, const Vector& direction)
	: m_Origin(origin), m_Direction(direction.GetNormalized())
{
	if (m_Direction.W != 0)
	{
		Error("Ray direction has a non-zero value of W!");
	}
}

void Ray::SetIntersection(Object* object, float distance)
{
	if (distance < m_IntersectionDistance)
	{
		m_HitObject = object;
		m_IntersectionDistance = distance;
	}
}

const Object* Ray::GetObject() const
{
	return m_HitObject;
}

Vector Ray::GetIntersectionPoint() const
{
	return m_Origin + m_Direction * m_IntersectionDistance;
}
