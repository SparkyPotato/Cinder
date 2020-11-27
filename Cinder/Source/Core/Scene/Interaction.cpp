#include "PCH.h"

#include "Interaction.h"

Interaction::Interaction(const Point& point, const Normal& normal, const Vector& error, const Vector& outgoing, float time)
	: Position(point), InteractionNormal(normal), Error(error), Outgoing(outgoing), Time(time)
{}

Interaction::Interaction(const Point& point, const Vector& outgoing, float time)
	: Position(point), Outgoing(outgoing), Time(time)
{}

Interaction::Interaction(const Point& point, float time)
	: Position(point), Time(time)
{}

bool Interaction::IsSurfaceInteraction() const
{
	return InteractionNormal != Normal();
}

bool Interaction::IsMediumInteraction() const
{
	return !IsSurfaceInteraction();
}

SurfaceInteraction::SurfaceInteraction(const Point& point, const Vector& error, const Point& parameticLocation, const Vector& outgoing, 
	const Vector& dpdu, const Vector& dpdv, const Normal& dndu, const Normal& dndv, float time, Shape* shape)
	: Interaction(point, Normal(Cross(dpdu, dpdv).Normalize()), error, outgoing, time), 
	ParametricLocation(ParametricLocation), PositionDerivativeU(dpdu), PositionDerivativeV(dpdv),
	NormalDerivativeU(dndu), NormalDerivativeV(dndv), InteractionWith(shape)
{
	Shading.InteractionNormal = InteractionNormal;
	Shading.PositionDerivativeU = PositionDerivativeU;
	Shading.PositionDerivativeV = PositionDerivativeV;
	Shading.NormalDerivativeU = NormalDerivativeU;
	Shading.NormalDerivativeV = NormalDerivativeV;
}
