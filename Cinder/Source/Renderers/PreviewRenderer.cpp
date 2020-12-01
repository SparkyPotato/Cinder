#include "PCH.h"
#include "PreviewRenderer.h"

REGISTER_RENDERER(Preview, PreviewRenderer)

PreviewRenderer::PreviewRenderer()
{
	Verbose("Preview Renderer created.");
}

bool PreviewRenderer::ParseSettings(const YAML::Node& node)
{
	if (!Sampler::ParseSettings(node)) { return false; }

	return true;
}

Color PreviewRenderer::TraceRay(const Ray& ray)
{
	if (m_Scene->AccelStructure->TestIntersect(ray))
	{
		return { 1.f, 0.f, 0.f };
	}

	return { 0.f, 0.f, 0.f };
}
