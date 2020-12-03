#include "PCH.h"
#include "WhittedRenderer.h"

REGISTER_RENDERER(Whitted, WhittedRenderer)

void WhittedRenderer::Render(const Scene& scene, Framebuffer& framebuffer)
{
	for (int i = 0; i < 6; i++)
	{
		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				m_Irradiance.Faces[i].GetPixel(x, y) =
					m_Irradiance.Sampler->Sample(scene.Skybox.Faces[i], float(x) / m_IrradianceResolution, float(y) / m_IrradianceResolution);
			}
		}
	}

	IrradianceSample average = GetAverageIrradiance(scene);
	GenerateIrradiance(average);

	Sampler::Render(scene, framebuffer);
}

bool WhittedRenderer::ParseSettings(const YAML::Node& node)
{
	if (!Sampler::ParseSettings(node)) { return false; }

	if (!node["Env Map Resolution"])
	{
		Warning("Env Map Resolution not specified (line {})! Using default ({}).", node.Mark().line + 1, m_IrradianceResolution);
	}
	else
	{
		try { m_IrradianceResolution = node["Env Map Resolution"].as<uint32_t>(); }
		catch (YAML::Exception& e)
		{
			Error("Env Map Resolution must be an unsigned integer (line {})!", e.mark.line + 1);
			return false;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		m_Irradiance.Faces[i].Width = m_IrradianceResolution;
		m_Irradiance.Faces[i].Height = m_IrradianceResolution;
		m_Irradiance.Faces[i].Data = Memory::Get()->AllocateArr<Color>(m_IrradianceResolution * m_IrradianceResolution);
	}

	m_Irradiance.Sampler = ComponentManager::Get()->SpawnTextureSampler("Bilinear");

	return true;
}

Color WhittedRenderer::TraceRay(const Ray& ray)
{
	return m_Irradiance(GCameraToWorld(ray.Direction));
}

WhittedRenderer::IrradianceSample WhittedRenderer::GetAverageIrradiance(const Scene& scene)
{
	IrradianceSample sum;
	for (int i = 0; i < 6; i++)
	{
		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				Vector direction = CubemapFaceData[i].FaceDirection +
					2.f * (float(x) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].UVector +
					2.f * (float(y) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].VVector;
				direction.Normalize();

				IrradianceSample sample;
				sample.Coefficients[0] = Color(0.282095f);
				sample.Coefficients[1] = Color(0.488603f * direction.X);
				sample.Coefficients[2] = Color(0.488603f * direction.Z);
				sample.Coefficients[3] = Color(0.488603f * direction.Y);
				sample.Coefficients[4] = Color(1.092548f * direction.X * direction.Z);
				sample.Coefficients[5] = Color(1.092548f * direction.Y * direction.Z);
				sample.Coefficients[6] = Color(1.092548f * direction.X * direction.Y);
				sample.Coefficients[7] = Color(0.315392f * (3.f * direction.Z * direction.Z - 1.f));
				sample.Coefficients[8] = Color(0.546274f * (direction.X * direction.X - direction.Y * direction.Y));

				sample *= m_Irradiance.Faces[i].GetPixel(x, y);
				sum += sample;
			}
		}
	}

	sum *= Color(4.f * Pi / (m_IrradianceResolution * m_IrradianceResolution * 6));
	return sum;
}

void WhittedRenderer::GenerateIrradiance(const IrradianceSample& average)
{
	for (int i = 0; i < 6; i++)
	{
		for (uint32_t y = 0; y < m_IrradianceResolution; y++)
		{
			for (uint32_t x = 0; x < m_IrradianceResolution; x++)
			{
				Vector direction = CubemapFaceData[i].FaceDirection +
					2.f * (float(x) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].UVector +
					2.f * (float(y) / m_IrradianceResolution - 0.5f) * CubemapFaceData[i].VVector;
				direction.Normalize();

				IrradianceSample sample;
				sample.Coefficients[0] = Color(0.282095f);
				sample.Coefficients[1] = Color(0.488603f * direction.X);
				sample.Coefficients[2] = Color(0.488603f * direction.Z);
				sample.Coefficients[3] = Color(0.488603f * direction.Y);
				sample.Coefficients[4] = Color(1.092548f * direction.X * direction.Z);
				sample.Coefficients[5] = Color(1.092548f * direction.Y * direction.Z);
				sample.Coefficients[6] = Color(1.092548f * direction.X * direction.Y);
				sample.Coefficients[7] = Color(0.315392f * (3.f * direction.Z * direction.Z - 1.f));
				sample.Coefficients[8] = Color(0.546274f * (direction.X * direction.X - direction.Y * direction.Y));

				Color& color = m_Irradiance.Faces[i].GetPixel(x, y);
				sample *= average;
				color = Color();
				for (int i = 0; i < 9; i++)
				{
					color += sample.Coefficients[i];
				}
			}
		}
	}
}

WhittedRenderer::IrradianceSample& WhittedRenderer::IrradianceSample::operator+=(const IrradianceSample& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] += other.Coefficients[i];
	}
	return *this;
}

WhittedRenderer::IrradianceSample WhittedRenderer::IrradianceSample::operator*=(const IrradianceSample& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] *= other.Coefficients[i];
	}
	return *this;
}

WhittedRenderer::IrradianceSample& WhittedRenderer::IrradianceSample::operator*=(const Color& other)
{
	for (int i = 0; i < 9; i++)
	{
		Coefficients[i] *= other;
	}
	return *this;
}
