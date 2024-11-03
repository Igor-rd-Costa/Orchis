file(GLOB_RECURSE SRC_FILES "Src/*.cpp" "Src/*.h")

add_library(OrchisRuntime SHARED ${SRC_FILES})

if(MSVC)
set_output_path(OrchisRuntime)
add_compile_options_shared(OrchisRuntime)
endif()

target_include_directories(OrchisRuntime
  PRIVATE "Src"
  PRIVATE "OrchisGraphicsBackend"
  PRIVATE "Vendor"
)

target_compile_definitions(OrchisRuntime 
  PRIVATE OC_SHARED_BUILD
)
