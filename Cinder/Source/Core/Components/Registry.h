#pragma once

class Renderer;
using RendererSpawnFunction = up<Renderer> (*)();
class Framebuffer;
using FramebufferSpawnFunction = up<Framebuffer> (*)(uint32_t, uint32_t);
class Camera;
class Transform;
using CameraSpawnFunction = up<Camera> (*)(const Transform&);
class Geometry;
using GeometrySpawnFunction = up<Geometry> (*)(const std::string&);
class Sampler;
using SamplerSpawnFunction = up<Sampler> (*)();
class AccelerationStructure;
using AccelerationSpawnFunction = up<AccelerationStructure>(*)();

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
