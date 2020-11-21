#pragma once

#include "Globals.h"
#include "Scene.h"

class Raytracer
{
public:
	Raytracer(const Scene& scene, Framebuffer& framebuffer)
		: m_Scene(scene), m_Framebuffer(framebuffer)
	{}

	void Run();
	void WritePixel(uint64_t x, uint64_t y, Vector rayDirection);
	Color Raycast(const Ray& ray);

private:
	const Scene& m_Scene;
	Framebuffer& m_Framebuffer;
};
