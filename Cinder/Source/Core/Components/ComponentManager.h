#pragma once

class Renderer;
class Framebuffer;

typedef Renderer* (*RendererSpawnFunction)();
typedef Framebuffer* (*FramebufferSpawnFunction)(uint32_t, uint32_t);

class ComponentManager
{
public:
	ComponentManager() = default;

	static ComponentManager* Get();

	Renderer* SpawnRenderer(const std::string& name);
	void RegisterRenderer(const char* name, RendererSpawnFunction function);

	Framebuffer* SpawnFramebuffer(const std::string& name, uint32_t width, uint32_t height);
	void RegisterFramebuffer(const char* name, FramebufferSpawnFunction function);

private:
	static ComponentManager* s_Singleton;

	std::map<std::string, RendererSpawnFunction> m_Renderers;
	std::map<std::string, FramebufferSpawnFunction> m_Framebuffers;
};
