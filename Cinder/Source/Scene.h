#pragma once

#include "json/json.hpp"

#include "Math/Ray.h"

struct SceneObject
{
	virtual bool Intersect(const Ray& ray, float& intersectionDistance) const = 0;
};

struct Sphere : SceneObject
{
	Sphere() = default;
	Sphere(const Vector& position, float radius);

	bool Intersect(const Ray& ray, float& intersectionDistance) const override;

	inline void SetRadius(float radius) { m_Radius = radius; m_RadiusSquare = radius * radius; }
	inline float GetRadius() const { return m_Radius; }
	inline void SetPosition(const Vector& position) { m_Position = position; }
	inline const Vector& GetPosition() const { return m_Position; }

private:
	Vector m_Position;
	float m_Radius;
	float m_RadiusSquare;
};

struct Scene
{
	std::vector<Sphere> Spheres;

	struct
	{
		Vector Position;
		Vector Direction;
		Vector Up;
		float VerticalFOV; // In radians
	} Camera;
};

void from_json(const nlohmann::json& j, Scene& scene);
void to_json(nlohmann::json& j, const Scene& scene);
