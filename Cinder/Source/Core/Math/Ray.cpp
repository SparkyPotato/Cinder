#include "PCH.h"
#include "Core/Math/Ray.h"

Ray::Ray(const Point& origin, const Vector& direction, float extent)
	: Origin(origin), Direction(direction), Extent(extent)
{
	ASSERT(extent > 0.f, "Extent of a ray cannot be negative!");
}

Point Ray::operator()(float distanceAlong) const
{
	ASSERT(distanceAlong > 0.f, "Cannot go backwards along ray!");

	return Origin + Direction * distanceAlong;
}

bool Ray::IsNAN() const
{
	return Origin.IsNAN() || Direction.IsNAN();
}
