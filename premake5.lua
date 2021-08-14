workspace "Fugly"
	configurations { "Debug", "Release" }

project "Fugly"
	location "Fugly"
	kind "ConsoleApp"
	language "C++"

	targetdir "Bin"
	objdir "Bin/Intermediates/"

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.hpp",	
		"%{prj.name}/Src/**.c",
		"%{prj.name}/Src/**.cc",
		"%{prj.name}/Src/**.cpp",
	}

	filter "configurations:Release"
		optimize "On"
