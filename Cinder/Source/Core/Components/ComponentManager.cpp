#include "PCH.h"
#include "ComponentManager.h"

ComponentManager* ComponentManager::s_Singleton = nullptr;

ComponentManager* ComponentManager::Get()
{
	if (s_Singleton) { return s_Singleton; }

	s_Singleton = new ComponentManager;
	return s_Singleton;
}

Renderer* ComponentManager::SpawnRenderer(const std::string& name)
{
	return m_Renderers.at(name)();
}

void ComponentManager::RegisterRenderer(const char* name, RendererSpawnFunction function)
{
	m_Renderers.emplace(name, function);
}

Framebuffer* ComponentManager::SpawnFramebuffer(const std::string& name, uint32_t width, uint32_t height)
{
	return m_Framebuffers.at(name)(width, height);
}

void ComponentManager::RegisterFramebuffer(const char* name, FramebufferSpawnFunction function)
{
	m_Framebuffers.emplace(name, function);
}

OutputAdapter* ComponentManager::SpawnOutputAdapter(const std::string& name, const std::string& filename)
{
	return m_OutputAdapters.at(name)(filename);
}

void ComponentManager::RegisterOutputAdapter(const char* name, OutputAdapterSpawnFunction function)
{
	m_OutputAdapters.emplace(name, function);
}

Shape* ComponentManager::SpawnShape(const std::string& type, const Transform& transform)
{
	return m_Shapes.at(type)(transform);
}

void ComponentManager::RegisterShape(const char* type, ShapeSpawnFunction function)
{
	m_Shapes.emplace(type, function);
}

AccelerationStructure* ComponentManager::SpawnAccelerationStructure(const std::string type)
{
	return m_AccelerationStructures.at(type)();
}

void ComponentManager::RegisterAccelerationStructure(const char* type, AccelerationSpawnFunction function)
{
	m_AccelerationStructures.emplace(type, function);
}
