# Cinder
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://opensource.org/licenses/MIT)

Cinder is a cross-platform, modular, and extendable ray-tracer. It supports Windows, macOS,
and should support any other POSIX-complaint OS. It has been tested on:

1. Windows 10 (version 20H2)
2. macOS Catalina (version 10.15.7)
3. Ubuntu 20 (version 20.04.1)

## Features
1. Easily-extendable component system composed of several simple interfaces.
2. Project and scene deserialization from YAML.
3. Sampler Renderer as a base for tile-based multithreaded raytracing.
4. Geometry-instancing, with spheres and triangle meshes.
5. Interface for object-ray intersection acceleration structures.
6. BRDF and BTDF interfaces, with Lambertian and Oren-Nayar diffuse BRDFs. Perfect specular reflection and transmission
   has also been implemented, with Fresnel reflectance (assuming unpolarized light) and refraction.
7. Texture interface, with constant-value, 8-bit, and floating-point textures.
8. Material interface for computing BSDFs. Perfect mirrors, glass, and matte materials have been implemented.
9. Light interface, with a point and directional light implemented.
10. Environment skyboxes, with runtime irradiance map generation, for diffuse IBL in the Whitted Renderer.

### Upcoming
1. BVH acceleration structure.
2. Area lights.
2. Monte Carlo path tracing.

## Performance
![](Content/Renders/Render1.png)
8K image rendered with 256 samples per pixel, in 136 minutes on an i7-4790K(4.6GHz all cores).
![](Content/Renders/Render2.png)
Blender's Suzanne, rendered in 4K with 8 samples per pixel, in 16 minutes on the same hardware as above.

## Getting Started
Cinder uses CMake.
1. Clone the repository, and fetch submodules:
``` 
git clone --recursive https://github.com/SparkyPotato/Cinder 
```
2. Run the CMake generator for your platform.
3. The binaries will be found in `Exectuable`, within the build folder.

### Windows
1. Open the folder with Visual Studio and create the desired configuration. Note that Cinder does not support x86.

### macOS
1. Run the CMake Xcode generator, or generate a Makefile. You can also use Ninja.

### Linux/POSIX
1. Run CMake for Ninja or Make.

## Dependencies
1. [fmt](https://github.com/fmtlib/fmt)
2. [yaml-cpp](https://github.com/jbeder/yaml-cpp)

All dependencies are added as git submodules.

## Using Cinder
To run the sample project, call Cinder with (assuming the build folder is located in the root folder):
```
./Cinder "../../SampleProjects/Project.yaml"
```
For more usage, check the [wiki](https://github.com/SparkyPotato/Cinder/wiki).
