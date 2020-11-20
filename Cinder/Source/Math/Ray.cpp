#include "Ray.h"

Ray::Ray(Vector origin, Vector direction)
	: m_Origin(origin), m_Direction(direction)
{
	m_Direction.Normalize();
}
