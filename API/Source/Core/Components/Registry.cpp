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
#include "Core/Components/Registry.h"

#include "Core/Components/Framebuffer.h"
#include "Core/Components/Renderer.h"
#include "Core/Scene/Camera.h"
#include "Core/Scene/Geometry.h"
#include "Core/Scene/Light.h"
#include "Core/Material/Texture.h"

Registry* Registry::s_Registry = nullptr;

Registry* Registry::Get()
{
	if (!s_Registry)
	{
		s_Registry = new Registry;
	}

	return s_Registry;
}

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

	return renderer->Parse(node);
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

	return framebuffer->Parse(node);
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

	return camera->Parse(node);
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

	return geometry->Parse(node);
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
	
	return texture->Parse(node);
}

bool YAML::convert<up<Light>>::decode(const Node& node, up<Light>& light)
{
	std::string type;
	try { type = node["Type"].as<std::string>(); }
	catch (YAML::Exception& e)
	{
		Error("Light type must be a string (line {})!", e.mark.line + 1);
		return false;
	}

	Transform t;
	try { t = node["Transform"].as<Transform>(); }
	catch (...) { return false; }

	uint32_t s;
	try { s = node["Samples"].as<uint32_t>(); }
	catch (YAML::Exception& e)
	{
		Error("Light samples must be an unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}

	try { light = Registry::Get()->GLights.at(type)(s, t); }
	catch (...)
	{
		Error("Light type '{}' does not exist (line {})!", type, node["Type"].Mark().line + 1);
		return false;
	}

	return light->Parse(node);
}
