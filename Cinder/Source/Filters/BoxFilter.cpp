#include "PCH.h"
#include "Filters/BoxFilter.h"

FILTER(Box, BoxFilter)

BoxFilter::BoxFilter(const Vector& radius)
	: Filter(radius)
{}

float BoxFilter::Evaluate(const Point& point)
{
	return 1.f;
}
