#include "Raytracer.h"

void Raytracer::Run()
{
	float aspectRatio = (float)m_Framebuffer.Width / m_Framebuffer.Height;

	Vector origin = m_Scene.Camera.Position;
	Vector direction = m_Scene.Camera.Direction;
	Vector up = m_Scene.Camera.Up;
	float vFOV = m_Scene.Camera.VerticalFOV;
	float tan = std::tan(vFOV / 2);

	Vector top = direction + up * direction.Length() * tan;
	Vector bottom = direction - up * direction.Length() * tan;
	Vector left = direction - Vector::Cross(up, direction) * tan * (aspectRatio);
	Vector right = direction + Vector::Cross(up, direction) * tan * (aspectRatio);

	ProgressBar bar(0, 100, 1);

	for (uint64_t y = 0; y < m_Framebuffer.Height; y++)
	{
		Vector verticalDirection = Vector::Lerp(top, bottom, (float)y / m_Framebuffer.Height);
		for (uint64_t x = 0; x < m_Framebuffer.Width; x++)
		{
			Vector horizontalDirection = Vector::Lerp(left, right, (float)x / m_Framebuffer.Width);

			WritePixel(x, y, horizontalDirection + verticalDirection);
		}
		bar.Update(y * 100 / m_Framebuffer.Height);
	}
	bar.End();
}

void Raytracer::WritePixel(uint64_t x, uint64_t y, Vector rayDirection)
{
	Ray cameraRay(m_Scene.Camera.Position, rayDirection);
	m_Framebuffer.GetPixel(x, y) = Raycast(cameraRay);
}

Vector Raytracer::Raycast(const Ray& ray)
{
	float closestHit = std::numeric_limits<float>().max();
	const Sphere* sphereHit = nullptr;

	for (auto& sphere : m_Scene.Spheres) 
	{ 
		if (sphere.Intersect(ray, closestHit)) { sphereHit = &sphere; };
	}

	if (sphereHit)
	{
		Vector hitPoint = ray.GetOrigin() + ray.GetDirection() * closestHit;
		Vector normal = (hitPoint - sphereHit->Position).Normalize();
		Vector objectColor = sphereHit->ObjMaterial->Color;
		
		float intensity = m_Scene.AmbientIntensity;
		for (auto& light : m_Scene.Lights)
		{
			Vector lightDirection = (light.Position - hitPoint).Normalize();
			if (ShadowTrace(Ray(hitPoint + normal * 0.01f, lightDirection)))
			{
				intensity += light.Intensity * std::max(0.f, Vector::Dot(lightDirection, normal));
			}
		}
		return objectColor * intensity;
	}

	return { 0.f, 0.f, 0.f }; // Background
}

bool Raytracer::ShadowTrace(const Ray& ray)
{
	float notNeeded;
	for (auto& sphere : m_Scene.Spheres)
	{
		if (sphere.Intersect(ray, notNeeded)) { return false; };
	}

	return true;
}
