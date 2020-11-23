#include "Sphere.h"

Sphere::Sphere(const Transform& transform, const std::string& material, float radius)
	: Object(transform, material), m_RadiusSquare(radius* radius)
{}

bool Sphere::Intersect(Ray& worldRay)
{
	// Transform ray to object space
	Vector origin = worldRay.GetOrigin() * m_Transform.GetTransformInverse();
	Vector direction = worldRay.GetDirection() * m_Transform.GetTransformInverse();

	float firstSolution, secondSolution;

	Vector length = -origin;
	float projectionDistance = Vector::Dot3D(length, direction);
	float distanceSquare = Vector::Dot3D(length, length) - projectionDistance * projectionDistance;
	if (distanceSquare > m_RadiusSquare) { return false; }
	float chordLength = sqrt(m_RadiusSquare - distanceSquare);
	firstSolution = projectionDistance - chordLength;
	secondSolution = projectionDistance + chordLength;

	if (firstSolution > secondSolution) { std::swap(firstSolution, secondSolution); }

	if (firstSolution < 0)
	{
		firstSolution = secondSolution;
		if (firstSolution < 0) { return false; }
	}

	worldRay.SetIntersection(this, firstSolution);
	return true;
}

bool Sphere::TestIntersect(const Ray& worldRay)
{
	// Transform ray to object space
	Vector origin = worldRay.GetOrigin() * m_Transform.GetTransformInverse();
	Vector direction = worldRay.GetDirection() * m_Transform.GetTransformInverse();

	Vector length = -origin;
	float projectionDistance = Vector::Dot3D(length, direction);
	float distanceSquare = Vector::Dot3D(length, length) - projectionDistance * projectionDistance;

	if (distanceSquare > m_RadiusSquare) { return false; }
	return true;
}
