#pragma once

#include "Core/Scene/Cubemap.h"
#include "Sampler.h"

class WhittedRenderer : public Sampler 
{
public:
	WhittedRenderer() = default;

	void Render(const Scene& scene, Framebuffer& framebuffer) override;
	bool ParseSettings(const YAML::Node& node) override;

	Color TraceRay(const Ray& ray) override;

private:
	struct IrradianceSample
	{
		Color Coefficients[9];

		IrradianceSample& operator+=(const IrradianceSample& other);
		IrradianceSample operator*=(const IrradianceSample& other);
		IrradianceSample& operator*=(const Color& other);
	};

	IrradianceSample GetAverageIrradiance(const Scene& scene);
	void GenerateIrradiance(const IrradianceSample& average);

	uint32_t m_IrradianceResolution = 128;
	Cubemap m_Irradiance;
};
