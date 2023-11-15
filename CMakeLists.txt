cmake_minimum_required(VERSION 3.16)

project(Orchis)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CONFIGURATION_TYPES Debug;Release)

add_compile_definitions(UNICODE)

if (WIN32)
    add_compile_definitions(OC_PLATFORM_WINDOWS)
endif()

if (WIN32)
    set(ORCHIS_PLATFORM "Windows")
endif()
set(ORCHIS_ARCHITECTURE "x86_64")
set(ORCHIS_BUILD_CONFIG "$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
set(ORCHIS_BIN_DIR ${CMAKE_SOURCE_DIR}/bin/)

add_compile_definitions(
    $<$<CONFIG:Debug>:OC_DEBUG_BUILD>
    $<$<CONFIG:Release>:OC_RELEASE_BUILD>
)

add_subdirectory(Core)
add_subdirectory(SandBox)

if (WIN32)
    add_subdirectory(OrchisRuntime)
    include_external_msproject(OrchisEditor OrchisEditor/OrchisEditor.csproj PLATFORM AnyCPU)
endif()