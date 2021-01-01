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
#include "Cameras/ThinLens.h"

#include "Core/Math/Sampling.h"

CAMERA(ThinLens, ThinLensCamera)

ThinLensCamera::ThinLensCamera(const Transform& transform)
	: Camera(transform)
{}

bool ThinLensCamera::Parse(const YAML::Node& node)
{
	if (!node["FOV"])
	{
		Error("Camera does not have an FOV (line {})!", node.Mark().line + 1);
		return false;
	}

	if (!node["Focal Length"])
	{
		Error("Camera does not have a focal length (line {})!", node.Mark().line + 1);
		return false;
	}

	if (!node["Aperture"])
	{
		Error("Camera does not have an aperture (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_FOV = node["FOV"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera FOV must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	try { m_FocalLength = node["Focal Length"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera focal length must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	try { m_Aperture = node["Aperture"].as<float>(); }
	catch (YAML::Exception& e)
	{
		Error("Camera aperture must be a float (line {})!", e.mark.line + 1);
		return false;
	}

	if (m_FOV > 90.f)
	{
		Warning("FOV greater than 90 degrees may lead to distorted images.");
	}

	m_FOV = ToRadians(m_FOV);
	m_FOV = tan(m_FOV / 2);
	m_FOV = tan(m_FOV / 2);

	return true;
}

Ray ThinLensCamera::GetRay(float u, float v, Sampler* sampler)
{
	float Px = (2.f * u - 1.f) * m_FOV * m_AspectRatio;
	float Py = (1.f - 2.f * v) * m_FOV;
	Point focus = Point() + Vector(Px, Py, 1.f) * m_FocalLength;
	
	Point sample = ConcentricSampleDisk(sampler->Get2D());
	std::swap(sample.Y(), sample.Z());
	Point origin = Point() + (sample - Point()) * m_Aperture;

	return Ray(origin, (focus - origin).GetNormalized());
}
