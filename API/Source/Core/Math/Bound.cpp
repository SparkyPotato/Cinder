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
#include "Core/Math/Bound.h"

Bound::Bound()
	: Minimum(FloatMin, FloatMin, FloatMin),
	Maximum(FloatMax, FloatMax, FloatMax)
{}

Bound::Bound(const Point &point)
	: Maximum(point), Minimum(point)
{}

Bound::Bound(const Point& point1, const Point& point2)
{
	Minimum = Point(
		std::min(point1.X(), point2.X()),
		std::min(point1.Y(), point2.Y()),
		std::min(point1.Z(), point2.Z())
	);
	
	Maximum = Point(
		std::max(point1.X(), point2.X()),
		std::max(point1.Y(), point2.Y()),
		std::max(point1.Z(), point2.Z())
	);
}

Vector Bound::GetDiagonal() const
{
	return Maximum - Minimum;
}

float Bound::GetSurfaceArea() const
{
	Vector diagonal = GetDiagonal();
	
	return 2 * (
		diagonal.X() * diagonal.Y() +
		diagonal.Y() * diagonal.Z() +
		diagonal.Z() * diagonal.X()
	);
}

float Bound::GetVolume() const
{
	Vector diagonal = GetDiagonal();
	
	return diagonal.GetLengthSquare();
}

uint8_t Bound::MaxExtent() const
{
	Vector diagonal = GetDiagonal();
	
	if (diagonal.X() > diagonal.Y() && diagonal.X() > diagonal.Z())
	{
		return 0;
	}
	else if(diagonal.Y() > diagonal.Z())
	{
		return  1;
	}
	else
	{
		return 2;
	}
}

Point Bound::Lerp(const Vector& point) const
{
	return Point(
		::Lerp(Minimum.X(), Maximum.X(), point.X()),
		::Lerp(Minimum.Y(), Maximum.Y(), point.Y()),
		::Lerp(Minimum.Z(), Maximum.Z(), point.Z())
	);
}

Vector Bound::Offset(const Point& point) const
{
	Vector diagonal = GetDiagonal();
	
	return Vector(
		(point.X() - Minimum.X()) / diagonal.X(),
		(point.Y() - Minimum.Y()) / diagonal.Y(),
		(point.Z() - Minimum.Z()) / diagonal.Z()
	);
}

bool Bound::Intersect(const Ray& ray, float& t0, float& t1) const
{
	t0 = 0.f; t1 = ray.Extent;
	
	Vector inverseDirection = { 1.f / ray.Direction.X(), 1.f / ray.Direction.Y(), 1.f / ray.Direction.Z() };
	Vector near = Hadamard((Minimum - ray.Origin), inverseDirection);
	Vector far = Hadamard((Maximum - ray.Origin), inverseDirection);
	
	Vector fars = Max(near, far);
	Vector nears = Min(near, far);
	
	for (int i = 0; i < 3; i++)
	{
		t0 = std::max(nears[i], t0);
		t1 = std::min(fars[i], t1);
		if (t0 > t1) { return false; }
	}
	
	return true;
}

bool Bound::Intersect(const Ray& ray, const Vector& inverseDirection, float& t0, float& t1) const
{
	t0 = 0.f; t1 = ray.Extent;
	
	Vector near = Hadamard((Minimum - ray.Origin), inverseDirection);
	Vector far = Hadamard((Maximum - ray.Origin), inverseDirection);
	
	Vector fars = Max(near, far);
	Vector nears = Min(near, far);
	
	for (int i = 0; i < 3; i++)
	{
		t0 = std::max(nears[i], t0);
		t1 = std::min(fars[i], t1);
		if (t0 > t1) { return false; }
	}
	
	return true;
}

void Bound::GetBoundingSphere(Point& outCenter, float& outRadius) const
{
	outCenter = Minimum + (Maximum - Minimum) / 2.f;
	outRadius = (Maximum - Minimum).GetLength() / 2.f;
}

bool Bound::Contains(const Point& point) const
{
	return point.X() >= Minimum.X() && point.X() <= Maximum.X() &&
		point.Y() >= Minimum.Y() && point.Y() <= Maximum.Y() &&
		point.Z() >= Minimum.Z() && point.Z() <= Maximum.Z();
}

bool Bound::Contains(const Bound& other) const
{
	return other.Minimum.X() >= Minimum.X() && other.Maximum.X() <= Maximum.X() &&
		other.Minimum.Y() >= Minimum.Y() && other.Maximum.Y() <= Maximum.Y() &&
		other.Minimum.Z() >= Minimum.Z() && other.Maximum.Z() <= Maximum.Z();
}

Bound Bound::GetEnlarged(float delta) const
{
	return Bound(
		Minimum - Vector(delta, delta, delta),
		Maximum + Vector(delta, delta, delta)
	);
}

Bound Bound::GetEnlarged(const Vector& delta) const
{
	return Bound(
		Minimum - delta,
		Maximum + delta
	);
}

Bound& Bound::Enlarge(float delta)
{
	Minimum -= Vector(delta, delta, delta);
	Maximum += Vector(delta, delta, delta);

	return *this;
}

Bound& Bound::Enlarge(const Vector& delta)
{
	Minimum -= delta;
	Maximum += delta;

	return *this;
}

bool operator==(const Bound& first, const Bound& second)
{
	return first.Minimum == second.Minimum && first.Maximum == second.Maximum;
}

bool operator!=(const Bound& first, const Bound& second)
{
	return !(first == second);
}

Bound Union(const Bound& bound, const Point& point)
{
	return Bound(
		{ 
			std::min(bound.Minimum.X(), point.X()),
			std::min(bound.Minimum.Y(), point.Y()),
			std::min(bound.Minimum.Z(), point.Z()) 
		},
		{ 
			std::max(bound.Maximum.X(), point.X()),
			std::max(bound.Maximum.Y(), point.Y()),
			std::max(bound.Maximum.Z(), point.Z()) 
		}
	);
}

Bound Union(const Bound& bound1, const Bound& bound2)
{
	return Bound(
		{
			std::min(bound1.Minimum.X(), bound2.Minimum.X()),
			std::min(bound1.Minimum.Y(), bound2.Minimum.Y()),
			std::min(bound1.Minimum.Z(), bound2.Minimum.Z())
		},
		{
			std::max(bound1.Maximum.X(), bound2.Maximum.X()),
			std::max(bound1.Maximum.Y(), bound2.Maximum.Y()),
			std::max(bound1.Maximum.Z(), bound2.Maximum.Z())
		}
	);
}

Bound Intersect(const Bound& bound1, const Bound& bound2)
{
	return Bound(
		{
			std::max(bound1.Minimum.X(), bound2.Minimum.X()),
			std::max(bound1.Minimum.Y(), bound2.Minimum.Y()),
			std::max(bound1.Minimum.Z(), bound2.Minimum.Z())
		},
		{
			std::min(bound1.Maximum.X(), bound2.Maximum.X()),
			std::min(bound1.Maximum.Y(), bound2.Maximum.Y()),
			std::min(bound1.Maximum.Z(), bound2.Maximum.Z())
		}
	);
}
