#pragma once

#include "Core/Material/Cubemap.h"
#include "Core//Math/Transform.h"

class Environment
{
public:
	Environment() = default;
	
	Color Sample(const Vector& direction) const;
	Color SampleIrradiance(const Vector& direction) const;

private:
	friend struct YAML::convert<Environment>;
	friend class Scene;

	struct IrradianceSample
	{
		Color Coefficients[9];

		IrradianceSample& operator+=(const IrradianceSample& other);
		IrradianceSample operator*=(const IrradianceSample& other);
		IrradianceSample& operator*=(const Color& other);
	};

	void ComputeIrradiance();

	IrradianceSample GetAverageIrradiance(const Cubemap& cubemap);
	void GenerateIrradiance(const IrradianceSample& average);

	uint32_t m_IrradianceResolution;
	Transform m_CameraToWorld;
	Cubemap m_Skybox;
	Cubemap m_Irradiance;
};

template<>
struct YAML::convert<Environment>
{
	static bool decode(const Node& node, Environment& environment);
};
