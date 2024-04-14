file(GLOB_RECURSE FILES 
    "./Core/*.cpp"
    "./Core/*.h"
    "./Debug/*.cpp"
    "./Debug/*.h"
    "./Platform/*.cpp"
    "./Platform/*.h"
    "./Renderer/*.cpp"
    "./Renderer/*.h"
    "./EditorRuntime/*.cpp"    
    "./EditorRuntime/*.h"    
)   

add_library(OrchisRuntime SHARED ${FILES})

target_compile_definitions(OrchisRuntime PRIVATE OC_SHARED_BUILD)

find_package(Vulkan REQUIRED)
target_include_directories(OrchisRuntime PRIVATE
    "./Include/PCH/"
    "./"
    "./Core/"
    "./Core/ECS/"
    "./vendor/spdlog/include/"
    "./vendor/glm/"
    "./vendor/stb/"
    "./vendor/assimp/include/"
    "${Vulkan_INCLUDE_DIRS}"
)

target_precompile_headers(OrchisRuntime PRIVATE "./Include/PCH/OrchisPCH.h")

target_compile_options(OrchisRuntime PRIVATE
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)
set_target_properties(OrchisRuntime PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${ORCHIS_BIN_DIR}/OrchisRuntime/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})
set(ASSIMP_LIB_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib/Debug/assimp-vc143-mtd.lib")
set(ASSIMP_LIB_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib/Release/assimp-vc143-mt.lib")
set(ZLIB_LIB_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/contrib/zlib/Debug/zlibstaticd.lib")
set(ZLIB_LIB_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/contrib/zlib/Release/zlibstatic.lib")

target_link_libraries(OrchisRuntime PRIVATE 
    ${Vulkan_LIBRARIES}
    $<$<CONFIG:Debug>:${ASSIMP_LIB_DIR_DEBUG} ${ZLIB_LIB_DIR_DEBUG}>
    $<$<CONFIG:Release>:${ASSIMP_LIB_DIR_RELEASE} ${ZLIB_LIB_DIR_RELEASE}>
)

set(ORCHIS_RUNTIME_BIN_DIR ${ORCHIS_BIN_DIR}OrchisRuntime/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})

message("CMD: ${ORCHIS_RUNTIME_BIN_DIR}")

add_custom_command(TARGET OrchisRuntime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${ORCHIS_RUNTIME_BIN_DIR}/OrchisRuntime.dll ${CMAKE_SOURCE_DIR}/OrchisEditor/bin/${ORCHIS_BUILD_CONFIG}/net8.0-windows10.0.22621.0/) 
add_custom_command(TARGET OrchisRuntime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${ORCHIS_RUNTIME_BIN_DIR}/OrchisRuntime.dll ${ORCHIS_BIN_DIR}/Sandbox/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE}/) 

set_target_properties(OrchisRuntime PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ORCHIS_BIN_DIR}/OrchisRuntime/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})