file(GLOB_RECURSE SRC_FILES "Src/*.cpp" "Src/*.h")

add_library(OrchisDX12GraphicsBackend SHARED ${SRC_FILES})

set_output_path(OrchisDX12GraphicsBackend)
add_compile_options_shared(OrchisDX12GraphicsBackend)

set_property(TARGET OrchisDX12GraphicsBackend PROPERTY FOLDER "OrchisGraphicsBackends")

target_compile_definitions(OrchisDX12GraphicsBackend PRIVATE OC_SHARED_BUILD)

target_include_directories(OrchisDX12GraphicsBackend
  PRIVATE "Src"
)