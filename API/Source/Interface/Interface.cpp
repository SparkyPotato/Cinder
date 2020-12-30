//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "PCH.h"

#include "fmt/chrono.h"

#include "Core/Components/Renderer.h"
#include "Geometry/Sphere.h"

#ifdef __cplusplus
extern "C" {
#endif

bool SetOptions(Options* options)
{
	GOptions = *options;
	return true;
}

bool RunCinderProject(const char* filePath, float* timeSeconds)
{
	std::filesystem::path path = filePath;

	try
	{
		auto start = std::chrono::high_resolution_clock().now();

		if (GOptions.ThreadCount == 0)
		{
			Log("Auto-detecting thread count.");
			GOptions.ThreadCount = std::thread::hardware_concurrency();
			// hardware_concurrency can return 0 if it failed to get the thread count from the OS
			if (GOptions.ThreadCount == 0)
			{
				Warning("Could not detect number of threads, using 4 as a reasonable default.");
				GOptions.ThreadCount = 4;
			}
		}

		Log("Rendering with {} threads.", GOptions.ThreadCount);

		YAML::Node project;
		try { project = YAML::LoadFile(path.string()); }
		catch (YAML::Exception& e)
		{
			Error("Failed to parse project file (line {})!", e.mark.line);
			return false;
		}

		// Set the working directory to the directory containing the project,
		// so we can use relative paths without issues.
		std::filesystem::path workingDirectory = std::filesystem::current_path();
		std::filesystem::current_path(path.parent_path());

		up<Renderer> renderer = nullptr;
		try { renderer = project["Renderer"].as<up<Renderer>>(); }
		catch (...) { return false; }

		up<Framebuffer> framebuffer = nullptr;
		try { framebuffer = project["Framebuffer"].as<up<Framebuffer>>(); }
		catch (...) { return false; }

		// Cinder supports a list of Scenes to render per project
		if (!project["Scenes"].IsSequence())
		{
			Error("Scene list must be a sequence (line {})!", project["Scenes"].Mark().line + 1);
			return false;
		}

		for (const auto& sceneIt : project["Scenes"])
		{
			std::string file;
			std::string output;
			try { file = sceneIt["File"].as<std::string>(); }
			catch (YAML::Exception& e)
			{
				Error("Failed to find Scene File (line {})!", e.mark.line + 1);
				return false;
			}
			try { output = sceneIt["Output"].as<std::string>(); }
			catch (YAML::Exception& e)
			{
				Error("Failed to find Scene Ouput (line {})!", e.mark.line + 1);
				return false;
			}

			// Load the scene and time it
			auto s = std::chrono::high_resolution_clock().now();
			Log("Loading Scene.");
			Scene* scene;
			try { scene = Scene::Load(file); }
			catch (YAML::Exception& e) { Error(e.what()); return false; }
			scene->SetCameraAspectRatio(float(framebuffer->Width) / framebuffer->Height);
			auto e = std::chrono::high_resolution_clock().now();
			Log("Loaded Scene. Took {:%M:%S}.", e - s);

			// Render and time it
			s = std::chrono::high_resolution_clock().now();
			Log("Rendering.");
			renderer->Render(*scene, *framebuffer);
			e = std::chrono::high_resolution_clock().now();
			Log("Rendered. Took {:%M:%S}.", e - s);

			// Output and time it
			s = std::chrono::high_resolution_clock().now();
			Log("Writing output.");
			framebuffer->Output(output);
			e = std::chrono::high_resolution_clock().now();
			Log("Written output. Took {:%M:%S}.", e - s);

			delete scene;
		}

		auto end = std::chrono::high_resolution_clock().now();

		*timeSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.f;

		// Reset the working directory so the next project can be loaded
		std::filesystem::current_path(workingDirectory);
	}
	catch (...) { return false; }

	return true;
}

#ifdef __cplusplus
}
#endif
