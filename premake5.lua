workspace "ngmath"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ngmath"
	location "ngmath"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ngmpch.h"
	pchsource "ngmath/src/ngmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

filter "system:windows"
	cppdialect "C++20"
	cdialect "c17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"NGM_PLATFORM_WINDOWS"
	}

	prebuildcommands
	{
		"{MKDIR} ../bin/" .. outputdir .. "/App"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/App")
	}

	filter "configurations:Debug"
		defines "NGM_CONFIG_DEBUG"
		buildoptions "/MTd"
		symbols "On"

	filter "configurations:Release"
		defines "NGM_CONFIG_RELEASE"
		buildoptions "/MT"
		optimize "On"

	filter "configurations:Dist"
		defines "NGM_CONFIG_DIST"
		buildoptions "/MT"
		optimize "On"

project "App"
	location "App"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"ngmath/src"
	}

	links
	{
		"ngmath"
	}

	filter "system:windows"
		cppdialect "c++20"
		cdialect "c17"
		staticruntime "On"
		systemversion "latest"

		filter "configurations:Debug"
		defines "NGM_CONFIG_DEBUG"
		buildoptions "/MTd"
		symbols "On"

	filter "configurations:Release"
		defines "NGM_CONFIG_RELEASE"
		buildoptions "/MT"
		optimize "On"

	filter "configurations:Dist"
		defines "NGM_CONFIG_DIST"
		buildoptions "/MT"
		optimize "On"