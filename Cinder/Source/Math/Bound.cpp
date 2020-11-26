#include "PCH.h"

#include "Bound.h"

Bound::Bound()
	: Minimum(FloatMin, FloatMin, FloatMin),
	Maximum(FloatMax, FloatMax, FloatMax)
{}

Bound::Bound(const Point &point)
	: Maximum(point), Minimum(point)
{}

Bound::Bound(const Point& point1, const Point& point2)
{
	Minimum = Point(
		std::min(point1.X, point2.X),
		std::min(point1.Y, point2.Y),
		std::min(point1.Z, point2.Z)
	);
	
	Maximum = Point(
		std::max(point1.X, point2.X),
		std::max(point1.Y, point2.Y),
		std::max(point1.Z, point2.Z)
	);
}

Vector Bound::GetDiagonal() const
{
	return Maximum - Minimum;
}

float Bound::GetSurfaceArea() const
{
	Vector diagonal = GetDiagonal();
	
	return 2 * (
		diagonal.X * diagonal.Y +
		diagonal.Y * diagonal.Z +
		diagonal.Z * diagonal.X
	);
}

float Bound::GetVolume() const
{
	Vector diagonal = GetDiagonal();
	
	return diagonal.GetLengthSquare();
}

uint8_t Bound::MaxExtent() const
{
	Vector diagonal = GetDiagonal();
	
	if (diagonal.X > diagonal.Y && diagonal.X > diagonal.Z)
	{
		return 0;
	}
	else if(diagonal.Y > diagonal.Z)
	{
		return  1;
	}
	else
	{
		return 2;
	}
}

Point Bound::Lerp(const Point& point) const
{
	return Point(
		::Lerp(Minimum.X, Maximum.X, point.X),
		::Lerp(Minimum.Y, Maximum.Y, point.Y),
		::Lerp(Minimum.Z, Maximum.Z, point.Z)
	);
}

Point Bound::Offset(const Point& point) const
{
	Vector diagonal = GetDiagonal();
	
	return Point(
				 point.X - Minimum.X
	);
}
