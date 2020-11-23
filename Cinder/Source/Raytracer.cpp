#include "Raytracer.h"

void Raytracer::Run()
{
	float aspectRatio = (float)m_Framebuffer.Width / m_Framebuffer.Height;

// 	Vector origin = m_Scene.MainCamera.Position;
// 	Vector direction = m_Scene.MainCamera.Direction;
// 	Vector up = m_Scene.MainCamera.Up;
// 	float vFOV = m_Scene.MainCamera.VerticalFOV;
// 	float tan = std::tan(vFOV / 2);
// 
// 	m_Top = direction + up * direction.Length() * tan;
// 	m_Bottom = direction - up * direction.Length() * tan;
// 	m_Left = direction - Vector::Cross(up, direction) * tan * (aspectRatio);
// 	m_Right = direction + Vector::Cross(up, direction) * tan * (aspectRatio);

	auto threadCount = std::thread::hardware_concurrency();
	if (CommandLine::Properties.count("threads")) { threadCount = std::stoi(CommandLine::Properties["threads"]); }

	if (threadCount == 0) { Warning("Thread count is 0. Using " COLOR "4\x1b[93m as a default."); threadCount = 4; }
	Output("Rendering using " COLOR, threadCount, "\x1b[0m threads.");

	m_Jobs = new std::vector<Job>[threadCount];
	m_CompletionBar = new ProgressBar(0, 100, 1);

	int thread = 0;
	for (uint64_t y = 0; y < m_Framebuffer.Height; y += 100)
	{
		for (uint64_t x = 0; x < m_Framebuffer.Width; x += 100)
		{
			if (x + 100 >= m_Framebuffer.Width || y + 100 >= m_Framebuffer.Height)
			{
				uint64_t xmax = x + 100, ymax = y + 100;
				if (xmax >= m_Framebuffer.Width) { xmax = m_Framebuffer.Width; }
				if (ymax >= m_Framebuffer.Height) { ymax = m_Framebuffer.Height; }

				m_Jobs[thread].emplace_back(x, y, xmax, ymax);
				m_JobCount++;
			}
			else
			{
				m_Jobs[thread].emplace_back(x, y, x + 100, y + 100);
				m_JobCount++;
			}

			thread++;
			if (thread == threadCount) { thread = 0; }
		}
	}

	for (uint8_t i = 0; i < threadCount; i++)
	{
		m_Threads.emplace_back(&Raytracer::Thread, this, i);
	}
	for (auto& thread : m_Threads)
	{
		thread.join();
	}

	m_CompletionBar->End();
	delete m_CompletionBar;
	delete[] m_Jobs;
}

Vector Raytracer::Raycast(const Ray& ray)
{
// 	float closestHit = std::numeric_limits<float>().max();
// 	const SceneObject* objectHit = nullptr;
// 
// 	for (auto object : m_Scene.Objects) 
// 	{ 
// 		if (object->Intersect(ray, closestHit)) { objectHit = object; };
// 	}
// 
// 	if (objectHit)
// 	{
// 		Vector hitPoint = ray.GetOrigin() + ray.GetDirection() * closestHit;
// 		Vector normal = objectHit->GetNormal(hitPoint);
// 		Vector objectColor = objectHit->ObjMaterial->Color;
// 
// 
// 
// 		return objectColor;
// 	}
// 
// 	return { 0.f, 0.f, 0.f }; // Background

	return { 0.f, 0.f, 0.f };
}

void Raytracer::Thread(uint8_t id)
{
// 	for (auto& job : m_Jobs[id])
// 	{
// 		for (uint64_t y = job.Ymin; y < job.Ymax; y++)
// 		{
// 			Vector verticalDirection = Vector::Lerp(m_Top, m_Bottom, (float)y / m_Framebuffer.Height);
// 			for (uint64_t x = job.Xmin; x < job.Xmax; x++)
// 			{
// 				Vector horizontalDirection = Vector::Lerp(m_Left, m_Right, (float)x / m_Framebuffer.Width);
// 
// 				Ray cameraRay(m_Scene.MainCamera.Position, horizontalDirection + verticalDirection);
// 				m_Framebuffer.GetPixel(x, y) = Raycast(cameraRay);
// 			}
// 		}
// 
// 		m_BarMutex.lock();
// 		m_DoneJobs++;
// 		m_CompletionBar->Update(m_DoneJobs * 100 / m_JobCount);
// 		m_BarMutex.unlock();
// 	}
}
