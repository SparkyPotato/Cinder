#include "PCH.h"
#include "BilinearSampler.h"

REGISTER_TEXTURE_SAMPLER(Bilinear, BilinearSampler)

Color BilinearSampler::Sample(const Texture& texture, float u, float v)
{
	if (u > 1.f || v > 1.f || u < 0.f || v < 0.f)
	{
		return Color(1.f, 0.f, 0.f);
	}

	u *= texture.Width - 1;
	v *= texture.Height - 1;

	auto Lu = uint32_t(std::floor(u));
	auto Lv = uint32_t(std::floor(v));
	auto Hu = uint32_t(std::ceil(u));
	auto Hv = uint32_t(std::ceil(v));
	float uRatio = u - Lu;
	float vRatio = v - Lv;

	Color utLerp = Lerp(texture.GetPixel(Lu, Lv), texture.GetPixel(Hu, Lv), uRatio);
	Color ubLerp = Lerp(texture.GetPixel(Lu, Hv), texture.GetPixel(Hu, Hv), uRatio);

	return Lerp(utLerp, ubLerp, vRatio);
}
