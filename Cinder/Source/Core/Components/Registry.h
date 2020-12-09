#pragma once

class Renderer;
using RendererSpawnFunction = up<Renderer> (*)();
class Framebuffer;
using FramebufferSpawnFunction = up<Framebuffer> (*)(uint32_t, uint32_t);
class Sampler;
using SamplerSpawnFunction = up<Sampler> (*)(uint32_t);
class Filter;
class Vector;
using FilterSpawnFunction = up<Filter> (*)(const Vector&);
class Camera;
class Transform;
using CameraSpawnFunction = up<Camera> (*)(const Transform&);
class Geometry;
using GeometrySpawnFunction = up<Geometry> (*)(const std::string&);
class AccelerationStructure;
using AccelerationSpawnFunction = up<AccelerationStructure> (*)();
class Texture;
using TextureSpawnFunction = up<Texture> (*)();
class Material;
using MaterialSpawnFunction = up<Material> (*)(const std::string&);

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
	std::map<std::string, FilterSpawnFunction> GFilters;
	std::map<std::string, TextureSpawnFunction> GTextures;
	std::map<std::string, MaterialSpawnFunction> GMaterials;

private:
	static Registry* s_Registry;
};
