file(GLOB_RECURSE FILES 
    "./Core/*.cpp"
    "./Core/*.h"
    "./Debug/*.cpp"
    "./Debug/*.h"
    "./Platform/*.cpp"
    "./Platform/*.h"
    "./Renderer/*.cpp"
    "./Renderer/*.h"
)   

add_library(Core STATIC ${FILES})

find_package(Vulkan REQUIRED)
target_include_directories(Core PRIVATE
    "./Include/PCH/"
    "./"
    "./Core/"
    "./vendor/spdlog/include/"
    "./vendor/glm/"
    "./vendor/stb/"
    "./vendor/assimp/include/"
    "${Vulkan_INCLUDE_DIRS}"
)

target_precompile_headers(Core PRIVATE "./Include/PCH/OrchisPCH.h")

target_compile_options(Core PRIVATE
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)
set_target_properties(Core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${ORCHIS_BIN_DIR}/Core/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})
set(ASSIMP_LIB_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib/Debug/assimp-vc143-mtd.lib")
set(ASSIMP_LIB_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib/Release/assimp-vc143-mt.lib")
set(ZLIB_LIB_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/contrib/zlib/Debug/zlibstaticd.lib")
set(ZLIB_LIB_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/contrib/zlib/Release/zlibstatic.lib")

target_link_libraries(Core PRIVATE 
    ${Vulkan_LIBRARIES}
    $<$<CONFIG:Debug>:${ASSIMP_LIB_DIR_DEBUG} ${ZLIB_LIB_DIR_DEBUG}>
    $<$<CONFIG:Release>:${ASSIMP_LIB_DIR_RELEASE} ${ZLIB_LIB_DIR_RELEASE}>
)