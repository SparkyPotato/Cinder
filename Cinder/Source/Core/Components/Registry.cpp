#include "PCH.h"
#include "Core/Components/Registry.h"

#include "Core/Components/Framebuffer.h"
#include "Core/Components/Renderer.h"
#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"
#include "Core/Material/Texture.h"

Registry* Registry::Get()
{
	if (!s_Registry)
	{
		s_Registry = new Registry;
	}

	return s_Registry;
}

Registry* Registry::s_Registry = nullptr;

bool YAML::convert<up<Renderer>>::decode(const Node& node, up<Renderer>& renderer)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Renderer type must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { renderer = Registry::Get()->GRenderers.at(type)(); }
	catch (...)
	{
		Error("Renderer type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	if (!renderer->Parse(node)) { return false; }

	return true;
}

bool YAML::convert<up<Framebuffer>>::decode(const Node& node, up<Framebuffer>& framebuffer)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Framebuffer type must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	uint32_t width, height;
	try { width = node["Resolution"][0].as<uint32_t>(); }
	catch (YAML::Exception& e)
	{
		Error("Framebuffer width must be unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}
	try { height = node["Resolution"][1].as<uint32_t>(); }
	catch (YAML::Exception& e)
	{
		Error("Framebuffer height must be unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}

	try { framebuffer = Registry::Get()->GFramebuffers.at(type)(width, height); }
	catch (...)
	{
		Error("Framebuffer type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	if (!framebuffer->Parse(node)) { return false; }

	return true;
}

bool YAML::convert<up<Camera>>::decode(const Node& node, up<Camera>& camera)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera type must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	Transform transform;
	try { transform = node["Transform"].as<Transform>(); }
	catch (...) { return false; }

	try { camera = Registry::Get()->GCameras.at(type)(transform); }
	catch (...)
	{
		Error("Camera type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	if (!camera->Parse(node)) { return false; }
	
	return true;
}

bool YAML::convert<up<Geometry>>::decode(const Node& node, up<Geometry>& geometry)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Geometry type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	std::string name;
	try { name = node["Name"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Geometry name must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	try { geometry = Registry::Get()->GGeometry.at(type)(name); }
	catch (...)
	{
		Error("Geometry type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	if (!geometry->Parse(node)) { return false; }

	return true;
}

bool YAML::convert<up<Texture>>::decode(const Node& node, up<Texture>& texture)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Texture type must be a string (line {})!", e.mark.line + 1);
		return false;
	}
	
	try { texture = Registry::Get()->GTextures.at(type)(); }
	catch (...)
	{
		Error("Texture type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}
	
	if (!texture->Parse(node)) { return false; }
	
	return true;
}
