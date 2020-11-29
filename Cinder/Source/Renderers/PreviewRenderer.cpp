#include "PCH.h"
#include "PreviewRenderer.h"

REGISTER_RENDERER(Preview, PreviewRenderer)

PreviewRenderer::PreviewRenderer()
{
	Verbose("Preview Renderer created.");
}

void PreviewRenderer::Render(const Scene& scene, Framebuffer& framebuffer)
{

}
