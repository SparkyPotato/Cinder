//    Copyright 2021 Shaye Garg
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

#pragma once

#include "Core/Components/Registry.h"
#include "Core/Components/Sampler.h"
#include "Core/Math/Ray.h"
#include "Core/Math/Transform.h"

/// Base class for cameras, which generate camera rays.
class Camera
{
public:
	/// Construct a camera.
	///
	/// \param toWorld Camera to world transformation.
	Camera(const Transform& toWorld)
		: ToWorld(toWorld)
	{}

	/// Virtual destructor.
	virtual ~Camera() = default;

	/// Set the aspect-ratio of the camera.
	///
	/// \param aspectRatio The aspect-ratio.
	void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	/// Parse the required parameters.
	///
	/// \param node The node containing the data.
	/// 
	/// \return If the parse succeeded.
	virtual bool Parse(const YAML::Node& node) = 0;

	/// Get a camera ray.
	///
	/// \param u U-value of the viewport (horizontal).
	/// \param v V-value of the viewport (vertical).
	/// \param sampler Sampler to use for generating the ray.
	/// 
	/// \return The generated ray.
	virtual Ray GetRay(float u, float v, Sampler* sampler) = 0;

	Transform ToWorld;

protected:
	float m_AspectRatio = 0.f;
};

template<>
struct YAML::convert<up<Camera>>
{
	static bool decode(const Node& node, up<Camera>&);
};

#define CAMERA(type, className) \
up<Camera> Spawn##className(const Transform& t) { return std::make_unique<className>(t); } \
struct Register##className \
{ \
	Register##className() { Registry::Get()->GCameras.emplace(#type, &Spawn##className); } \
}; \
static Register##className StaticRegister##className;
