workspace "Xeph2D"
	architecture "x64"
	configurations
	{
		"Debug",
		"Debug (Console)",
		"Editor",
		"Editor (Console)",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ==============================

project "Xeph2D"
	location "%{prj.name}"
	kind "StaticLib"
	language "C++"
	targetname "%{prj.name}"
	targetdir ("bin/".. outputdir)
	objdir ("%{prj.name}/int/".. outputdir)
	cppdialect "C++17"
	staticruntime "Off"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/include",
		"box2d/include",
		"SFML/include"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "NCONSOLE" }
		symbols "On"

	filter "configurations:Debug (Console)"
		defines { "_DEBUG", "_CONSOLE" }
		symbols "On"
		
	filter "configurations:Editor"
		defines { "_DEBUG", "NCONSOLE", "_EDITOR" }
		symbols "On"
		includedirs { "ImGui/include" }

	filter "configurations:Editor (Console)"
		defines { "_DEBUG", "_CONSOLE", "_EDITOR" }
		symbols "On"
		includedirs { "ImGui/include" }
		
	filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
		
-- ==============================

project "App"
	location "%{prj.name}"
	kind "WindowedApp"
	language "C++"
	targetname "%{prj.name}"
	targetdir ("bin/".. outputdir)
	objdir ("%{prj.name}/int/".. outputdir)
	cppdialect "C++17"
	staticruntime "Off"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/Assets/Scripts",
		"box2d/include",
		"SFML/include",
		"Xeph2D/include"
	}
	
	libdirs
	{
		"%{prj.name}/lib"
	}
	
	links { "Xeph2D" }
	
	filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "NCONSOLE" }
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib",
			"sfml-audio-d.lib",
			"sfml-network-d.lib"
		}

	filter "configurations:Debug (Console)"
		defines { "_DEBUG", "_CONSOLE" }
		kind "ConsoleApp"
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib",
			"sfml-audio-d.lib",
			"sfml-network-d.lib"
		}
		
	filter "configurations:Editor"
		defines { "_DEBUG", "NCONSOLE", "_EDITOR" }
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib",
			"sfml-audio-d.lib",
			"sfml-network-d.lib",
			"imgui.lib",
			"opengl32"
		}

	filter "configurations:Editor (Console)"
		defines { "_DEBUG", "_CONSOLE", "_EDITOR" }
		kind "ConsoleApp"
		symbols "On"
		links
		{
			"box2d-d.lib",
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib",
			"sfml-audio-d.lib",
			"sfml-network-d.lib",
			"imgui.lib",
			"opengl32"
		}
		
	filter "configurations:Release"
		defines { "NDEBUG", "NCONSOLE" }
		optimize "On"
		links
		{
			"box2d.lib",
			"sfml-system.lib",
			"sfml-graphics.lib",
			"sfml-window.lib",
			"sfml-audio.lib",
			"sfml-network.lib"
		}