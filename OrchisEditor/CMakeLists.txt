file(GLOB_RECURSE SRC_FILES "Src/*.cpp" "Src/*.h")
add_executable(OrchisEditor WIN32 ${SRC_FILES})

if (MSVC)
set_output_path(OrchisEditor)
add_compile_options_static(OrchisEditor)
endif()

target_link_libraries(OrchisEditor OrchisRuntime)

target_include_directories(OrchisEditor
	PRIVATE "../OrchisRuntime/Include"
)