#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Vector.h"

class Filter
{
public:
	Filter(const Vector& radius)
		: m_Radius(radius), m_InverseRadius(1.f / radius.X(), 1.f / radius.Y(), 0.f)
	{}

	virtual float Evaluate(const Point& point) = 0;

protected:
	const Vector m_Radius, m_InverseRadius;
};

#define FILTER(type, className) \
up<Filter> Spawn##className(const Vector& radius) { return std::make_unique<className>(radius); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GFilters.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
