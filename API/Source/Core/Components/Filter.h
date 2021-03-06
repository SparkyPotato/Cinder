//    Copyright 2021 Shaye Garg
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "Core/Components/Registry.h"
#include "Core/Math/Vector.h"

/// Base class of image filters.
/// Image filters are used in image reconstruction, 
/// to weight the contribution of samples relative to the center of the pixel.
class Filter
{
public:
	/// Construct the filter.
	///
	/// \param radius The radius of the effect of the filter.
	Filter(const Vector& radius)
		: m_Radius(radius), m_InverseRadius(1.f / radius.X(), 1.f / radius.Y(), 0.f)
	{}

	/// Virtual destructor.
	virtual ~Filter() = default;

	/// Evaluate the filter function.
	///
	/// \param point The point of the sample, relative to the center of the pixel.
	///  
	/// \return The weight of the sample.
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
