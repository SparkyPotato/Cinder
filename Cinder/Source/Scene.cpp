#include "Scene.h"

#include "Globals.h"
#include "Math/Math.h"

Sphere::Sphere(const Vector& position, float radius)
	: m_Position(position), m_Radius(radius)
{
	m_RadiusSquare = m_Radius * m_Radius;
}

bool Sphere::Intersect(const Ray& ray, float& intersectionDistance) const
{
	float firstSolution, secondSolution;

	Vector length = m_Position - ray.GetOrigin();
	float projectionDistance = Vector::Dot(length, ray.GetDirection());
	float distanceSquare = Vector::Dot(length, length) - projectionDistance * projectionDistance;
	if (distanceSquare > m_RadiusSquare) return false;
	float chordLength = sqrt(m_RadiusSquare - distanceSquare);
	firstSolution = projectionDistance - chordLength;
	secondSolution = projectionDistance + chordLength;

	if (firstSolution > secondSolution) { std::swap(firstSolution, secondSolution); }

	if (firstSolution < 0) 
	{
		firstSolution = secondSolution;
		if (firstSolution < 0) { return false; }
	}

	intersectionDistance = firstSolution;
	return true;
}

void from_json(const nlohmann::json& j, Sphere& sphere)
{
	sphere.SetPosition(j.at("Position").get<Vector>());
	sphere.SetRadius(j.at("Radius").get<float>());
}

void to_json(nlohmann::json& j, const Sphere& sphere)
{
	j["Position"] = sphere.GetPosition();
	j["Radius"] = sphere.GetRadius();
}

void from_json(const nlohmann::json& j, Scene& scene)
{
	scene.Camera.Position = j.at("Camera").at("Position").get<Vector>();
	scene.Camera.Direction = j.at("Camera").at("Direction").get<Vector>();
	scene.Camera.Direction.Normalize(); // Ensure view direction is normalized
	scene.Camera.Up = j.at("Camera").at("Up").get<Vector>();
	scene.Camera.Up.Normalize();
	scene.Camera.VerticalFOV = ToRadians(j.at("Camera").at("VerticalFOV").get<float>());

	if (Vector::Dot(scene.Camera.Direction, scene.Camera.Up) != 0)
	{
		Error("Camera up vector and direction vector are not perpendicular!");
	}

	scene.Spheres = j.at("Objects").at("Spheres").get<std::vector<Sphere>>();
}

void to_json(nlohmann::json& j, const Scene& scene)
{
	j["Camera"]["Position"] = scene.Camera.Position;
	j["Camera"]["Direction"] = scene.Camera.Direction;
	j["Camera"]["VerticalFOV"] = ToDegrees(scene.Camera.VerticalFOV);

	j["Objects"]["Spheres"] = scene.Spheres;
}
