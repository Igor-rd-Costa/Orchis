file(GLOB_RECURSE SRC_FILES "Src/*.cpp" "Src/*.h")

add_library(OrchisVulkanGraphicsBackend SHARED ${SRC_FILES})

set_output_path(OrchisVulkanGraphicsBackend)
add_compile_options_shared(OrchisVulkanGraphicsBackend)


set_property(TARGET OrchisVulkanGraphicsBackend PROPERTY FOLDER "OrchisGraphicsBackends")

find_package(Vulkan REQUIRED)

target_compile_definitions(OrchisVulkanGraphicsBackend PRIVATE OC_SHARED_BUILD)

target_include_directories(OrchisVulkanGraphicsBackend
  PRIVATE "Src"
  PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(OrchisVulkanGraphicsBackend ${Vulkan_LIBRARIES})