#pragma once

#include "Vector.h"

// Axis-aligned bounding box
class Bound
{
public:
	Bound();
	Bound(const Point& point);
	Bound(const Point& point1, const Point& point2);
	
	Vector GetDiagonal() const;
	float GetSurfaceArea() const;
	float GetVolume() const;
	
	// Index of the largest extent of the box:
	// x: 0, y: 1, z: 2
	uint8_t MaxExtent() const;
	// Lerp between the corners of the box,
	// Value of all elements of point should be between 0.f and 1.f
	// to stay in the box
	Point Lerp(const Point& point) const;
	// Gets the offset of the point from Minimum, but
	// the offset of maximum is (1, 1, 1), not the diagonal.
	// Opposite of Lerp, pretty much
	Point Offset(const Point& point) const;
	
	void GetBoundingSphere(Point& outCenter, float& outRadius) const;
	
	Point Minimum, Maximum;
};

bool operator==(const Bound& first, const Bound& second);
bool operator!=(const Bound& first, const Bound& second);

Bound Union(const Bound& bound, const Point& point);
Bound Union(const Bound& bound1, const Bound& bound2);
Bound Intersect(const Bound& bound1, const Bound& bound2);

bool IsInside(const Bound& larger, const Bound& smaller);
bool IsInside(const Bound& bound, const Point& point);

Bound Enlarge(const Bound& bound, float delta);
Bound Enlarge(const Bound& bound, const Vector& delta);
