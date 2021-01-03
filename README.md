# Cinder
![GitHub](https://img.shields.io/github/license/SparkyPotato/Cinder)
[![Documentation Status](https://readthedocs.org/projects/cinder/badge/?version=latest)](https://cinder.readthedocs.io/en/latest/?badge=latest)

Cinder is a modular and extensible raytracer.

![](Docs/source/Static/CornellBox.png)
Cornell Box rendered with Cinder, at a resolution of 1200x1200, with 128 samples per pixel.

![](Docs/source/Static/MitsubaDepthOfField.png)
Cornell Box rendered with [Mitsuba 2] (https://www.mitsuba-renderer.org), at a resolution of 1200x1200, with 128 samples per pixel.

## Features
Implemented features include:
1. Framework for ray-intersection acceleration structures.
2. Lambertian, Oren-Nayar, Cook-Torrance, and Specular BRDFs.
3. Microfacet and Fresnel adapters for reflection and transmission.
4. Camera framework, with a simple perspective camera.
5. Sampler framework, with a random sampler.
6. Image reconstruction filter, with a box filter.
7. Spheres and triangle mesh geometry.
8. Point, directional, area, and environmental lights.
9. Emissive, glass, matte, metal, mirror, and plastic materials.
10. Recursive Whitted renderer, and unbiased Monte-Carlo path tracer.
11. 8-bit, HDR, and constant textures.

## Using Cinder
Visit the documentation at [ReadTheDocs](https://cinder.readthedocs.io/)
