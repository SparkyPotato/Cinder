#include "Scene.h"

#include "Globals.h"
#include "Math/Math.h"

Sphere::Sphere(const Vector& position, float radius)
	: Position(position), Radius(radius)
{
	RadiusSquare = Radius * Radius;
}

bool Sphere::Intersect(const Ray& ray, float& intersectionDistance) const
{
	float firstSolution, secondSolution;

	Vector length = Position - ray.GetOrigin();
	float projectionDistance = Vector::Dot(length, ray.GetDirection());
	float distanceSquare = Vector::Dot(length, length) - projectionDistance * projectionDistance;
	if (distanceSquare > RadiusSquare) return false;
	float chordLength = sqrt(RadiusSquare - distanceSquare);
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

bool operator==(const Sphere& first, const Sphere& second)
{
	return first.Position == second.Position &&
		first.Radius == second.Radius;
}

bool operator!=(const Sphere& first, const Sphere& second)
{
	return !(first == second);
}

bool operator==(const Material& material, const std::string& name)
{
	return material.Name == name;
}

bool operator!=(const Material& material, const std::string& name)
{
	return material.Name != name;
}

void from_json(const nlohmann::json& j, Sphere& sphere)
{
	sphere.Position = j.at("Position").get<Vector>();
	sphere.Radius = j.at("Radius").get<float>();
	sphere.RadiusSquare = sphere.Radius * sphere.Radius;
	sphere.MaterialName = j.at("Material").get<std::string>();
}

void from_json(const nlohmann::json& j, Material& material)
{
	material.Color = j.at("Color").get<Vector>();
	material.Name = j.at("Name").get<std::string>();
}

void from_json(const nlohmann::json& j, Light& light)
{
	light.Position = j.at("Position").get<Vector>();
	light.Color = j.at("Color").get<Vector>();
	light.Intensity = j.at("Intensity").get<float>();
}

void from_json(const nlohmann::json& j, Scene& scene)
{
	scene.Camera.Position = j.at("Camera").at("Position").get<Vector>();
	scene.Camera.Direction = j.at("Camera").at("Direction").get<Vector>();
	scene.Camera.Direction.Normalize(); // Ensure view direction is normalized
	scene.Camera.Up = j.at("Camera").at("Up").get<Vector>();
	scene.Camera.Up.Normalize();
	scene.Camera.VerticalFOV = ToRadians(j.at("Camera").at("VerticalFOV").get<float>());

	scene.AmbientIntensity = j.at("Global").at("AmbientIntensity").get<float>();

	if (Vector::Dot(scene.Camera.Direction, scene.Camera.Up) != 0)
	{
		Error("Camera up vector and direction vector are not perpendicular!");
	}

	scene.Materials = j.at("Materials").get<std::vector<Material>>();
	scene.Lights = j.at("Lights").get<std::vector<Light>>();
	scene.Spheres = j.at("Objects").at("Spheres").get<std::vector<Sphere>>();

	for (auto& sphere : scene.Spheres)
	{
		auto it = std::find(scene.Materials.begin(), scene.Materials.end(), sphere.MaterialName);
		if (it != scene.Materials.end()) { sphere.ObjMaterial = &(*it); }
		else { Error("Material '" COLOR, sphere.MaterialName.c_str(), "\x1b[31m' does not exist."); }
	}
}
