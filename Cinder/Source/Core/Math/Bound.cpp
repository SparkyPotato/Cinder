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
		std::min(point1.GetX(), point2.GetX()),
		std::min(point1.GetY(), point2.GetY()),
		std::min(point1.GetZ(), point2.GetZ())
	);
	
	Maximum = Point(
		std::max(point1.GetX(), point2.GetX()),
		std::max(point1.GetY(), point2.GetY()),
		std::max(point1.GetZ(), point2.GetZ())
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
		diagonal.GetX() * diagonal.GetY() +
		diagonal.GetY() * diagonal.GetZ() +
		diagonal.GetZ() * diagonal.GetX()
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
	
	if (diagonal.GetX() > diagonal.GetY() && diagonal.GetX() > diagonal.GetZ())
	{
		return 0;
	}
	else if(diagonal.GetY() > diagonal.GetZ())
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
		::Lerp(Minimum.GetX(), Maximum.GetX(), point.GetX()),
		::Lerp(Minimum.GetY(), Maximum.GetY(), point.GetY()),
		::Lerp(Minimum.GetZ(), Maximum.GetZ(), point.GetZ())
	);
}

Vector Bound::Offset(const Point& point) const
{
	Vector diagonal = GetDiagonal();
	
	return Vector(
		(point.GetX() - Minimum.GetX()) / diagonal.GetX(),
		(point.GetY() - Minimum.GetY()) / diagonal.GetY(),
		(point.GetZ() - Minimum.GetZ()) / diagonal.GetZ()
	);
}

void Bound::GetBoundingSphere(Point& outCenter, float& outRadius) const
{
	outCenter = Minimum + (Maximum - Minimum) / 2.f;
	outRadius = (Maximum - Minimum).GetLength() / 2.f;
}

bool Bound::Contains(const Point& point) const
{
	return point.GetX() >= Minimum.GetX() && point.GetX() <= Maximum.GetX() &&
		point.GetY() >= Minimum.GetY() && point.GetY() <= Maximum.GetY() &&
		point.GetZ() >= Minimum.GetZ() && point.GetZ() <= Maximum.GetZ();
}

bool Bound::Contains(const Bound& other) const
{
	return other.Minimum.GetX() >= Minimum.GetX() && other.Maximum.GetX() <= Maximum.GetX() &&
		other.Minimum.GetY() >= Minimum.GetY() && other.Maximum.GetY() <= Maximum.GetY() &&
		other.Minimum.GetZ() >= Minimum.GetZ() && other.Maximum.GetZ() <= Maximum.GetZ();
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
			std::min(bound.Minimum.GetX(), point.GetX()),
			std::min(bound.Minimum.GetY(), point.GetY()),
			std::min(bound.Minimum.GetZ(), point.GetZ()) 
		},
		{ 
			std::max(bound.Maximum.GetX(), point.GetX()),
			std::max(bound.Maximum.GetY(), point.GetY()),
			std::max(bound.Maximum.GetZ(), point.GetZ()) 
		}
	);
}

Bound Union(const Bound& bound1, const Bound& bound2)
{
	return Bound(
		{
			std::min(bound1.Minimum.GetX(), bound2.Minimum.GetX()),
			std::min(bound1.Minimum.GetY(), bound2.Minimum.GetY()),
			std::min(bound1.Minimum.GetZ(), bound2.Minimum.GetZ())
		},
		{
			std::max(bound1.Maximum.GetX(), bound2.Maximum.GetX()),
			std::max(bound1.Maximum.GetY(), bound2.Maximum.GetY()),
			std::max(bound1.Maximum.GetZ(), bound2.Maximum.GetZ())
		}
	);
}

Bound Intersect(const Bound& bound1, const Bound& bound2)
{
	return Bound(
		{
			std::max(bound1.Minimum.GetX(), bound2.Minimum.GetX()),
			std::max(bound1.Minimum.GetY(), bound2.Minimum.GetY()),
			std::max(bound1.Minimum.GetZ(), bound2.Minimum.GetZ())
		},
		{
			std::min(bound1.Maximum.GetX(), bound2.Maximum.GetX()),
			std::min(bound1.Maximum.GetY(), bound2.Maximum.GetY()),
			std::min(bound1.Maximum.GetZ(), bound2.Maximum.GetZ())
		}
	);
}
