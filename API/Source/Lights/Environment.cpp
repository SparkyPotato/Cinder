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

#include "PCH.h"
#include "Lights/Environment.h"

#include "Core/Scene/Scene.h"
#include "Textures/HDRTexture.h"

LIGHT(Environment, Environment)

Environment::Environment(uint32_t samples, const Transform& transform)
	: Light(samples, transform, false)
{}

Color Environment::Sample(const Interaction& interaction, Sampler* sampler, Vector& incoming, float& pdf, Occlusion& tester) const
{
	incoming = Vector(interaction.SNormal);
	pdf = 1.f;
	tester.Point1 = interaction;
	tester.Point2.HitPoint = interaction.HitPoint + Vector(interaction.SNormal) * Epsilon * 2.f;

	return m_Irradiance(ToCamera.GetInverse()(Vector(interaction.SNormal)));
}

Color Environment::EvaluateAlong(const Ray& ray) const
{
	return m_Skybox(ToCamera.GetInverse()(ray.Direction));
}

bool Environment::Parse(const YAML::Node& node)
{
	try { m_Skybox = node.as<Cubemap>(); }
	catch (...) { return false; }

	if (!node["Radiance Map Resolution"])
	{
		Error("Environment does not have radiance map resolution (line {})!", node.Mark().line + 1);
		return false;
	}

	try { m_IrradianceResolution = node["Radiance Map Resolution"].as<uint32_t>(); }
	catch (YAML::Exception& e)
	{
		Error("Radiance map resolution must be an unsigned integer (line {})!", e.mark.line + 1);
		return false;
	}

	return true;
}

void Environment::Preprocess(const Scene& scene)
{
	ComputeIrradiance();
}

void Environment::ComputeIrradiance()
{
	up<Texture> downscaled[6];

	for (int i = 0; i < 6; i++)
	{
		auto data = new Color[m_IrradianceResolution * m_IrradianceResolution];

		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				Color c = m_Skybox(
					Vector(-CubemapFaceData[i].FaceNormal) +
					CubemapFaceData[i].UVector * ((float(x) / m_IrradianceResolution) * 2.f - 1.f) +
					CubemapFaceData[i].VVector * ((float(y) / m_IrradianceResolution) * 2.f - 1.f)
				);

				data[y * m_IrradianceResolution + x] = c;
			}
		}

		downscaled[i] = std::make_unique<HDRTexture>(data, m_IrradianceResolution, m_IrradianceResolution);
		delete[] data;
	}

	auto average = GetAverageIrradiance(Cubemap(downscaled));
	GenerateIrradiance(average);
}

Environment::IrradianceSample Environment::GetAverageIrradiance(const Cubemap& cubemap)
{
	IrradianceSample sum;
	for (int i = 0; i < 6; i++)
	{
		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				Vector direction = Vector(-CubemapFaceData[i].FaceNormal) +
					2.f * (float(x) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].UVector +
					2.f * (float(y) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].VVector;
				direction.Normalize();

				IrradianceSample sample;
				sample.Coefficients[0] = Color(0.282095f);
				sample.Coefficients[1] = Color(0.488603f * direction.X());
				sample.Coefficients[2] = Color(0.488603f * direction.Z());
				sample.Coefficients[3] = Color(0.488603f * direction.Y());
				sample.Coefficients[4] = Color(1.092548f * direction.X() * direction.Z());
				sample.Coefficients[5] = Color(1.092548f * direction.Y() * direction.Z());
				sample.Coefficients[6] = Color(1.092548f * direction.X() * direction.Y());
				sample.Coefficients[7] = Color(0.315392f * (3.f * direction.Z() * direction.Z() - 1.f));
				sample.Coefficients[8] = Color(0.546274f * (direction.X() * direction.X() - direction.Y() * direction.Y()));

				sample *= cubemap(
					Vector(-CubemapFaceData[i].FaceNormal) +
					CubemapFaceData[i].UVector * ((float(x) / m_IrradianceResolution) * 2.f - 1.f) +
					CubemapFaceData[i].VVector * ((float(y) / m_IrradianceResolution) * 2.f - 1.f)
				);

				sum += sample;
			}
		}
	}

	sum *= Color(4.f * Pi / (m_IrradianceResolution * m_IrradianceResolution * 6));
	return sum;
}

void Environment::GenerateIrradiance(const IrradianceSample& average)
{
	up<Texture> irradiance[6];

	for (int i = 0; i < 6; i++)
	{
		auto data = new Color[m_IrradianceResolution * m_IrradianceResolution];

		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				Vector direction = Vector(-CubemapFaceData[i].FaceNormal) +
					2.f * (float(x) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].UVector +
					2.f * (float(y) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].VVector;
				direction.Normalize();

				IrradianceSample sample;
				sample.Coefficients[0] = Color(0.282095f);
				sample.Coefficients[1] = Color(0.488603f * direction.X());
				sample.Coefficients[2] = Color(0.488603f * direction.Z());
				sample.Coefficients[3] = Color(0.488603f * direction.Y());
				sample.Coefficients[4] = Color(1.092548f * direction.X() * direction.Z());
				sample.Coefficients[5] = Color(1.092548f * direction.Y() * direction.Z());
				sample.Coefficients[6] = Color(1.092548f * direction.X() * direction.Y());
				sample.Coefficients[7] = Color(0.315392f * (3.f * direction.Z() * direction.Z() - 1.f));
				sample.Coefficients[8] = Color(0.546274f * (direction.X() * direction.X() - direction.Y() * direction.Y()));

				sample *= average;
				Color& color = data[y * m_IrradianceResolution + x];
				color = Color();
				for (int i = 0; i < 9; i++)
				{
					color += sample.Coefficients[i];
				}
				color.R() = std::max(0.f, color.R());
				color.G() = std::max(0.f, color.G());
				color.B() = std::max(0.f, color.B());
			}
		}

		irradiance[i] = std::make_unique<HDRTexture>(data, m_IrradianceResolution, m_IrradianceResolution);
		delete[] data;
	}

	m_Irradiance = Cubemap(irradiance);
}

Environment::IrradianceSample& Environment::IrradianceSample::operator+=(const IrradianceSample& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] += other.Coefficients[i];
	}
	return *this;
}

Environment::IrradianceSample Environment::IrradianceSample::operator*=(const IrradianceSample& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] *= other.Coefficients[i];
	}
	return *this;
}

Environment::IrradianceSample& Environment::IrradianceSample::operator*=(const Color& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] *= other;
	}
	return *this;
}

