workspace "Cinder"
	architecture "x86_64"
	startproject "Cinder"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
project "Cinder"
	location "Cinder"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	characterset  "Unicode"
	
	targetdir "Binaries/%{cfg.buildcfg}/Cinder"
	objdir "Intermediate/%{cfg.buildcfg}/Cinder"
	
	links
	{
		"fmt",
		"yaml-cpp"
	}
	
	files
	{
		"Cinder/Source/**.h",
		"Cinder/Source/**.cpp"
	}
	
	includedirs
	{
		"Cinder/Source",
		"Cinder/Dependencies/fmt/include",
		"Cinder/Dependencies/stb",
		"Cinder/Dependencies/yaml-cpp/include"
	}
	
	pchsource "Cinder/Source/PCH.cpp"
	
	filter "action:vs*"
		pchheader "PCH.h"
	
	filter "action:xcode4"
		pchheader "Source/PCH.h"
		
	filter "action:gmake2"
		pchheader "Cinder/Source/PCH.h"
		linkoptions "-lpthread"
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "CFG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		runtime "Release"
		optimize "on"

group "Dependencies"
project "fmt"
	location "Cinder/Dependencies/ProjectFiles"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	characterset  "Unicode"
	
	targetdir("Binaries/%{cfg.buildcfg}/fmt")
	objdir("Intermediate/%{cfg.buildcfg}/fmt")
	
	files
	{
		"Cinder/Dependencies/fmt/src/**.cc"
	}
	
	includedirs
	{
		"Cinder/Dependencies/fmt/include",
		"Cinder/Dependencies/fmt"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "CFG_DEBUG"
		symbols "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		optimize "on"
		
project "yaml-cpp"
	location "Cinder/Dependencies/ProjectFiles"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"
	
	characterset  "Unicode"
	
	targetdir("Binaries/%{cfg.buildcfg}/yaml-cpp")
	objdir("Intermediate/%{cfg.buildcfg}/yaml-cpp")
	
	files
	{
		"Cinder/Dependencies/yaml-cpp/src/**.h",
		"Cinder/Dependencies/yaml-cpp/src/**.cpp"
	}
	
	includedirs
	{
		"Cinder/Dependencies/yaml-cpp/include",
		"Cinder/Dependencies/yaml-cpp"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "CFG_DEBUG"
		symbols "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		optimize "on"	
