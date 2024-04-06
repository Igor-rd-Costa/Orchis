file (GLOB_RECURSE FILES 
    "*.cpp"
    "*.h"
)

add_executable(Sandbox ${FILES})

target_include_directories(Sandbox PRIVATE
    "../Core/Include/Export/"
    "../Core/"
    "../Core/Core/"
    "../Core/vendor/glm/"
)

target_compile_options(Sandbox PRIVATE
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)
set_target_properties(Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ORCHIS_BIN_DIR}/Sandbox/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})

target_link_libraries(Sandbox PRIVATE OrchisRuntime)