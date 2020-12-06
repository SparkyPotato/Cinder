#pragma once

#include "Core/Components/Filter.h"

class BoxFilter : public Filter
{
public:
	BoxFilter(const Vector& radius);

	virtual float Evaluate(const Point& point) override;
};
