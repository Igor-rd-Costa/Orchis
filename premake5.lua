workspace "Orchis"
    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "Sandbox"

    defines
    {
        "ORCHIS_PLATFORM_WINDOWS"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	staticruntime "on"
    location "Core"

    files 
    {
        "Core/Core/**.cpp",
        "Core/Core/**.h",
        "Core/Include/PCH/OrchisPCH.cpp",
        "Core/Include/PCH/OrchisPCH.h",
        "Core/Include/Export/Orchis.h"
    }

    includedirs 
    {
        "Core/Include/PCH",
        "Core/Core"
    }

    pchheader "OrchisPCH.h"
    pchsource "Core/Include/PCH/OrchisPCH.cpp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        files
        {
            "Core/Platform/Windows/**.cpp",
            "Core/Platform/Windows/**.h"
        }

project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
	staticruntime "on"
    location "Sandbox"

    includedirs
    {
        "Core/Include/Export",
        "Core/Core"
    }
    
    files 
    {
        "Sandbox/src/**.cpp",
        "Sandbox/src/**.h"
    }

    links
    {
        "Core"
    }