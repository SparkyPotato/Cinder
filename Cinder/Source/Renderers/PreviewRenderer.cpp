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
