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
	RayIntersection r;
	if (m_Scene->AccelStructure->Intersect(ray, r))
	{
		return r.HitObject->ObjectMaterial->SampleAlbedo(r.U, r.V);
	}

	return m_Scene->Skybox(GCameraToWorld(ray.Direction));
}
