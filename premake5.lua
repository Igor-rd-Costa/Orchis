workspace "Orchis"
configurations { "Debug", "Release", "Dist" }
architecture "x64"
startproject "Sandbox"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["Vulkan"] = "C:/VulkanSDK/1.3.250.1/Include"
    IncludeDir["spdlog"] = "Core/vendor/spdlog/include"
    IncludeDir["glm"] = "Core/vendor/glm"
    IncludeDir["stb"] = "Core/vendor/stb"
    IncludeDir["assimp"] = "Core/vendor/assimp/include"
    
    VulkanLibDir = "C:/VulkanSDK/1.3.250.1/Lib"
    AssimpLibDir = "Core/vendor/assimp/lib/Release"
    ZlibDir = "Core/vendor/assimp/contrib/zlib/Release"

    defines
    {
        "OC_BUILD_STATIC"
    }

    filter "system:windows"
        defines
        {
            "OC_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines
        {
            "OC_DEBUG_BUILD"
        }
        
    filter "configurations:Release"
        defines
        {
            "OC_RELEASE_BUILD"
        }
        
    filter "configurations:Dist"
        defines
        {
            "OC_DIST_BUILD"
        }
        

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
        "Core/Debug/**.cpp",
        "Core/Debug/**.h",
        "Core/Include/PCH/OrchisPCH.cpp",
        "Core/Include/PCH/OrchisPCH.h",
        "Core/Include/Export/Orchis.h",
        "Core/Platform/Vulkan/*.cpp",
        "Core/Platform/Vulkan/*.h",
        "Core/Renderer/*.cpp",
        "Core/Renderer/*.h",
        "Core/Assets/Textures/**.png",
        "Core/Assets/Textures/**.jpg",
        "Core/Assets/Shaders/**.vert",
        "Core/Assets/Shaders/**.frag",
        "Core/Assets/Shaders/**.bat",
        "Core/Assets/Shaders/bin/**.spv",
        "Core/vendor/stb/**.h"
    }

    includedirs 
    {
        "Core/Include/PCH",
        "Core",
        "Core/Core",
        "%{IncludeDir.Vulkan}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.assimp}"
    }

    libdirs
    {
        "%{VulkanLibDir}",
        "%{AssimpLibDir}",
        "%{ZlibDir}"
    }

    links
    {
        "vulkan-1.lib",
        "assimp-vc143-mt.lib",
        "zlibstatic.lib"
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

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs
    {
        "Core/Include/Export",
        "Core",
        "Core/Core",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}"
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