#pragma once

#include "Vector.h"

class Medium;

// A ray in 3D with an origin and direction.
// Direction is normalized in the constructor
class Ray
{
public:
	Ray() = default;
	Ray(const Point& origin, const Vector& direction, float extent = FloatMax, const Medium* medium = nullptr);
	Ray(const Ray& other) = default;

	Point operator()(float distanceAlong) const;

	bool IsNAN() const;

	Point Origin;
	Vector Direction;
	float Extent = FloatMax;
	uint64_t Frame = 0;
	const Medium* CurrentMedium = nullptr;
};

// Difference between the rays of two samples
// INCOMPLETE
class RayDifferential : public Ray
{
public:
	RayDifferential() = default;
	RayDifferential(const Ray& ray);
	RayDifferential(const Point& origin, const Vector& direction, float extent = FloatMax, const Medium* medium = nullptr);
	RayDifferential(const RayDifferential& other) = default;

	bool IsNAN() const;

	bool HasDifferential = false;
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

template<>
struct fmt::formatter<RayDifferential>
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
			"Origin: {" + ParseString + "}, Direction: {" + ParseString + "}, Extent: {" + ParseString + "}, "
			"Has Differential: {}";

		return end;
	}

	template<typename FormatContext>
	auto format(const RayDifferential& ray, FormatContext& context)
	{
		return format_to(
			context.out(),
			ParseString,
			ray.Origin, ray.Direction, ray.Extent, ray.HasDifferential
		);
	}
};
