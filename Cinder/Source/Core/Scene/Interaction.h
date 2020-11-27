#pragma once

#include "Core/Math/Vector.h"
#include "Ray.h"

class Shape;

struct Interaction
{
	Interaction() = default;
	Interaction(const Point& point, const Normal& normal, const Vector& error, const Vector& outgoing, float time);
	Interaction(const Point& point, const Vector& outgoing, float time);
	Interaction(const Point& point, float time);

	bool IsSurfaceInteraction() const;
	bool IsMediumInteraction() const;

	Point Position;
	float Time = 0.f;
	Vector Outgoing;
	Vector Error;
	Normal InteractionNormal;
};

struct SurfaceInteraction : Interaction
{
	SurfaceInteraction() = default;
	SurfaceInteraction(const Point& point, const Vector& error, const Point& parameticLocation,
		const Vector& outgoing, const Vector& dpdu, const Vector& dpdv, const Normal& dndu, const Normal& dndv,
		float time, Shape* shape);

	Point ParametricLocation;
	Vector PositionDerivativeU, PositionDerivativeV;
	Normal NormalDerivativeU, NormalDerivativeV;
	Shape* InteractionWith;

	struct 
	{
		Normal InteractionNormal;
		Vector PositionDerivativeU, PositionDerivativeV;
		Normal NormalDerivativeU, NormalDerivativeV;
	} Shading;
};
