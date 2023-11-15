file (GLOB_RECURSE FILES
    "./src/*.cpp"
    "./src/*.h"
)

add_library(OrchisRuntime SHARED ${FILES})

target_include_directories(OrchisRuntime PRIVATE
    "../Core/Include/Export/"
    "../Core/"
    "../Core/Core/"
    "../Core/vendor/glm/"
)

target_compile_options(OrchisRuntime PRIVATE
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)

set(ORCHIS_RUNTIME_BIN_DIR ${ORCHIS_BIN_DIR}OrchisRuntime/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})

message("CMD: ${ORCHIS_RUNTIME_BIN_DIR}")

add_custom_command(TARGET OrchisRuntime POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${ORCHIS_RUNTIME_BIN_DIR}/OrchisRuntime.dll ${CMAKE_SOURCE_DIR}/OrchisEditor/bin/${ORCHIS_BUILD_CONFIG}/net7.0-windows/) 

target_link_libraries(OrchisRuntime PRIVATE Core)

set_target_properties(OrchisRuntime PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ORCHIS_BIN_DIR}/OrchisRuntime/${ORCHIS_BUILD_CONFIG}-${ORCHIS_PLATFORM}-${ORCHIS_ARCHITECTURE})
