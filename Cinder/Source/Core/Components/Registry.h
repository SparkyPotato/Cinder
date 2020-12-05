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
class Sampler;
using SamplerSpawnFunction = Sampler* (*)();
class AccelerationStructure;
using AccelerationSpawnFunction = AccelerationStructure* (*)();

class Registry
{
public:
	static Registry* Get();

	std::map<std::string, RendererSpawnFunction> GRenderers;
	std::map<std::string, FramebufferSpawnFunction> GFramebuffers;
	std::map<std::string, CameraSpawnFunction> GCameras;
	std::map<std::string, GeometrySpawnFunction> GGeometry;
	std::map<std::string, SamplerSpawnFunction> GSamplers;
	std::map<std::string, AccelerationSpawnFunction> GAcceleration;

private:
	static Registry* s_Registry;
};
