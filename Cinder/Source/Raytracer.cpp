#include "Raytracer.h"

void Raytracer::Run()
{
	float aspectRatio = (float)m_Framebuffer.Width / m_Framebuffer.Height;
	m_AspectX = tan(m_Scene.GetCamera().GetFOV() / 2) * aspectRatio;
	m_AspectY = tan(m_Scene.GetCamera().GetFOV() / 2);

	m_CameraPosition = Vector() * m_Scene.GetCamera().GetCameraToWorldMatrix();

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

void Raytracer::Thread(uint8_t id)
{
	for (auto& job : m_Jobs[id])
	{
		for (uint64_t y = job.Ymin; y < job.Ymax; y++)
		{
			float Py = (1.f - 2.f * ((y + 0.5f) / m_Framebuffer.Height)) * m_AspectY;

			for (uint64_t x = job.Xmin; x < job.Xmax; x++)
			{
				float Px = (2.f * ((x + 0.5f) / m_Framebuffer.Width) - 1.f) * m_AspectX;
				Vector rayDirection = Vector(Px, Py, 1.f) * m_Scene.GetCamera().GetCameraToWorldMatrix()
					- m_CameraPosition;
				
				m_Framebuffer.GetPixel(x, y) = Raycast(Ray(m_CameraPosition, rayDirection));
			}
		}

		m_BarMutex.lock();
		m_DoneJobs++;
		m_CompletionBar->Update(m_DoneJobs * 100 / m_JobCount);
		m_BarMutex.unlock();
 	}
}

Color Raytracer::Raycast(Ray ray)
{
	for (auto object : m_Scene.GetObjects())
	{
		object->Intersect(ray);
	}

	if (ray.GetObject())
	{
		return ray.GetObject()->GetMaterial()->GetAlbedo();
	}
	return Color(0.f, 0.f, 0.f);
}
