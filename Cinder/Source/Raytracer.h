#pragma once

#include <mutex>
#include <thread>

#include "Globals.h"
#include "Math/Ray.h"
#include "Scene/Scene.h"

class Raytracer
{
public:
	Raytracer(const Scene& scene, Framebuffer& framebuffer)
		: m_Scene(scene), m_Framebuffer(framebuffer)
	{}

	void Run();
	Vector Raycast(const Ray& ray);

	void Thread(uint8_t id);

private:
	const Scene& m_Scene;
	Framebuffer& m_Framebuffer;
	
	struct Job
	{
		Job(uint64_t xmin, uint64_t ymin, uint64_t xmax, uint64_t ymax)
			: Xmin(xmin), Ymin(ymin), Xmax(xmax), Ymax(ymax)
		{}

		uint64_t Xmin = 0, Xmax = 0;
		uint64_t Ymin = 0, Ymax = 0;
	};

	Vector m_Top, m_Bottom, m_Left, m_Right;

	ProgressBar* m_CompletionBar = nullptr;
	std::mutex m_BarMutex;
	uint64_t m_JobCount = 0;
	uint64_t m_DoneJobs = 0;

	std::vector<Job>* m_Jobs = nullptr;
	std::vector<std::thread> m_Threads;
};
