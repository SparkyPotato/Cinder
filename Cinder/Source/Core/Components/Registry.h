#pragma once

class Renderer;
using RendererSpawnFunction = Renderer* (*)();
class Framebuffer;
using FramebufferSpawnFunction = Framebuffer* (*)(uint32_t, uint32_t);
class Camera;
class Transform;
using CameraSpawnFunction = Camera* (*)(const Transform&);
class Geometry;
using GeometrySpawnFunction = Geometry* (*)(const std::string&);

class Registry
{
public:
	static Registry* Get();

	std::map<std::string, RendererSpawnFunction> GRenderers;
	std::map<std::string, FramebufferSpawnFunction> GFramebuffers;
	std::map<std::string, CameraSpawnFunction> GCameras;
	std::map<std::string, GeometrySpawnFunction> GGeometry;

private:
	static Registry* s_Registry;
};
