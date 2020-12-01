#include "PCH.h"
#include "Ray.h"

Ray::Ray(const Point& origin, const Vector& direction, float extent)
	: Origin(origin), Direction(direction.GetNormalized()), Extent(extent)
{
	ASSERT(extent > 0.f, "Extent of a ray cannot be negative!");
}

Point Ray::operator()(float distanceAlong) const
{
	return Origin + Direction * distanceAlong;
}

bool Ray::IsNAN() const
{
	return Origin.IsNAN() || Direction.IsNAN();
}
