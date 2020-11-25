#include "PCH.h"

#include "Ray.h"

Ray::Ray(const Point& origin, const Vector& direction, float extent, const Medium* medium)
	: Origin(origin), Direction(direction.GetNormalized()), Extent(extent), CurrentMedium(medium)
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

RayDifferential::RayDifferential(const Point& origin, const Vector& direction, float extent, const Medium* medium)
	: Ray(origin, direction, extent, medium)
{}

RayDifferential::RayDifferential(const Ray& ray)
	: Ray(ray)
{}

bool RayDifferential::IsNAN() const
{
	return Ray::IsNAN();
}
