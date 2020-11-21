#pragma once

#include "json/json.hpp"

#include "Math/Ray.h"

struct SceneObject
{
	virtual bool Intersect(const Ray& ray, float& intersectionDistance) const = 0;
};

struct Material
{
	std::string Name;

	Vector Color;
};

struct Light
{
	Vector Position;
	Vector Color;
	float Intensity;
};

struct Sphere : SceneObject
{
	Sphere() = default;
	Sphere(const Vector& position, float radius);

	bool Intersect(const Ray& ray, float& intersectionDistance) const override;

	Vector Position;
	float Radius;
	float RadiusSquare;
	Material* ObjMaterial;
	std::string MaterialName;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
	std::vector<Light> Lights;

	float AmbientIntensity;

	struct
	{
		Vector Position;
		Vector Direction;
		Vector Up;
		float VerticalFOV; // In radians
	} Camera;
};

bool operator==(const Sphere& first, const Sphere& second);
bool operator!=(const Sphere& first, const Sphere& second);
bool operator==(const Material& material, const std::string& name);
bool operator!=(const Material& material, const std::string& name);

void from_json(const nlohmann::json& j, Scene& scene);
void to_json(nlohmann::json& j, const Scene& scene);
