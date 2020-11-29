#pragma once

#include "Core/Math/Vector.h"

// A ray in 3D with an origin and direction.
// Direction is normalized in the constructor
class Ray
{
public:
	Ray() = default;
	Ray(const Point& origin, const Vector& direction, float extent = FloatMax);
	Ray(const Ray& other) = default;

	Point operator()(float distanceAlong) const;

	bool IsNAN() const;

	Point Origin;
	Vector Direction;
	mutable float Extent = FloatMax;
	float Time = 0.f;
};

template<>
struct fmt::formatter<Ray>
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
			"Origin: {" + ParseString + "}" + ", Direction: {" + ParseString + "}, Extent: {" + ParseString + "}";

		return end;
	}

	template<typename FormatContext>
	auto format(const Ray& ray, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			ray.Origin, ray.Direction, ray.Extent
		);
	}
};
