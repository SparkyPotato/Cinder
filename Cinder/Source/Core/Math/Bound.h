#pragma once

#include "Core/Math/Vector.h"

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
	Point Lerp(const Vector& point) const;
	// Gets the offset of the point from Minimum, but
	// the offset of maximum is (1, 1, 1), not the diagonal.
	// Opposite of Lerp, pretty much
	Vector Offset(const Point& point) const;
	
	void GetBoundingSphere(Point& outCenter, float& outRadius) const;

	bool Contains(const Point& point) const;
	bool Contains(const Bound& other) const;

	// Returns an enlarged version
	Bound GetEnlarged(float delta) const;
	Bound GetEnlarged(const Vector& delta) const;

	// Enlarges in place
	Bound& Enlarge(float delta);
	Bound& Enlarge(const Vector& delta);
	
	Point Minimum, Maximum;
};

bool operator==(const Bound& first, const Bound& second);
bool operator!=(const Bound& first, const Bound& second);

Bound Union(const Bound& bound, const Point& point);
Bound Union(const Bound& bound1, const Bound& bound2);
Bound Intersect(const Bound& bound1, const Bound& bound2);

template<>
struct fmt::formatter<Bound>
{
	std::string ParseString;

	auto parse(format_parse_context& context)
	{
		auto end = std::find(context.begin(), context.end(), '}');

		if (end != context.end())
		{
			ParseString = std::string(context.begin(), end);
		}

		ParseString =
			"Minimum: {" + ParseString + "}" + ", Maxiumum: {" + ParseString + "}";

		return end;
	}

	template<typename FormatContext>
	auto format(const Bound& bound, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			bound.Minimum, bound.Maximum
		);
	}
};
