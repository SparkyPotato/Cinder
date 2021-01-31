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

#include "PCH.h"
#include "Core/Math/Ray.h"

Ray::Ray(const Point& origin, const Vector& direction, float extent)
	: Origin(origin), Direction(direction), Extent(extent)
{
	ASSERT(extent > 0.f, "Extent of a ray cannot be negative!");
}

Point Ray::operator()(float distanceAlong) const
{
	ASSERT(distanceAlong > 0.f, "Cannot go backwards along ray!");

	return Origin + Direction * distanceAlong;
}

bool Ray::IsNAN() const
{
	return Origin.IsNAN() || Direction.IsNAN();
}
