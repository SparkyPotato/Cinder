#pragma once

class Renderer;
class Framebuffer;
class OutputAdapter;
class Shape;
class Transform;

typedef Renderer* (*RendererSpawnFunction)();
typedef OutputAdapter* (*OutputAdapterSpawnFunction)(const std::string&);
typedef Framebuffer* (*FramebufferSpawnFunction)(uint32_t, uint32_t);
typedef Shape* (*ShapeSpawnFunction)(const Transform&);

class ComponentManager
{
public:
	ComponentManager() = default;

	static ComponentManager* Get();

	Renderer* SpawnRenderer(const std::string& name);
	void RegisterRenderer(const char* name, RendererSpawnFunction function);

	Framebuffer* SpawnFramebuffer(const std::string& name, uint32_t width, uint32_t height);
	void RegisterFramebuffer(const char* name, FramebufferSpawnFunction function);

	OutputAdapter* SpawnOutputAdapter(const std::string& name, const std::string& filename);
	void RegisterOutputAdapter(const char* name, OutputAdapterSpawnFunction function);

	Shape* SpawnShape(const std::string& type, const Transform& transform);
	void RegisterShape(const char* type, ShapeSpawnFunction function);

private:
	static ComponentManager* s_Singleton;

	std::map<std::string, RendererSpawnFunction> m_Renderers;
	std::map<std::string, FramebufferSpawnFunction> m_Framebuffers;
	std::map<std::string, OutputAdapterSpawnFunction> m_OutputAdapters;
	std::map<std::string, ShapeSpawnFunction> m_Shapes;
};
