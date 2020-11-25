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
		"Cinder/Dependencies/yaml-cpp/include",
		"Cinder/Dependencies/stb"
	}
	
	filter "system:windows"
		pchheader "PCH.h"
		pchsource "Cinder/Source/PCH.cpp"
		systemversion "latest"
	
	filter "system:osx"
		pchheader "Source/PCH.h"
		
		
	filter "configurations:Debug"
		defines "CFG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		runtime "Release"
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
