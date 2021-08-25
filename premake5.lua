workspace "Fugly"
	configurations { "Debug", "Release" }
	platforms { "x86_64" }


project "Fugly"
	location "Fugly"
	kind "ConsoleApp"
	language "C++"

	targetdir "Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	objdir "Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Intermediates"

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.hpp",	
		"%{prj.name}/Src/**.c",
		"%{prj.name}/Src/**.cc",
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/Res/**.*",
	}

	includedirs
	{
		"Dependencies/Includes",
		"Fugly/Src/Vendor"
	}

	defines
	{
		"GLEW_STATIC"
	}

	links
	{
		"glfw3.lib",
		"glew32s.lib",
		"opengl32.lib",
		"assimp.lib"
	}

	filter "platforms:x86"
		libdirs
		{
			"Dependencies/Libs32"
		}

	filter "platforms:x86_64"
		libdirs
		{
			"Dependencies/Libs64"
		}

	filter "configurations:Release"
		optimize "On"

-- TODO: postbuild command to copy assimp.dll to .exe directory
