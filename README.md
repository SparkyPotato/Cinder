# Cinder
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://opensource.org/licenses/MIT)

Cinder is a cross-platform, modular, and extendable ray-tracer. It supports Windows, macOS,
and should support any other POSIX-complaint OS. It has been tested on:

1. Windows 10 (version 20H2)
2. macOS Catalina (version 10.15.7)
3. Ubuntu 20 (version 20.04.1)

## Getting Started
1. Clone the repository, and fetch submodules:
``` 
git clone --recursive https://github.com/SparkyPotato/Cinder 
```
2. Follow platform-specific steps
3. The binaries will be found in ` Binaries/Cinder/ `.

### Windows
1. Navigate to the ` Scripts/ ` directory, and run ` GenerateWindows.bat `.
2. Open the generated Visual Studio 2019 solution, and build normally.

### macOS
1. Navigate to the ` Scripts/ ` directory, and run ` GenerateMac.command `.
2. Open the Xcode workspace.
3. Ensure the target is set to ` Cinder `.
4. Build.

### Linux/POSIX
1. Navigate to the ` Scripts/ ` directory, and run ` GenerateLinux.sh `.
2. Navigate back to the repository root and run make:
```
make config=debug/release
```
3. Ensure that ` config ` is set to either ` debug ` or ` release `.

## Using Cinder
To run the sample project, call Cinder with
```
./Cinder "../../../SampleProjects/Project.yaml"
```
For more usage, check the [wiki](https://github.com/SparkyPotato/Cinder/wiki).
