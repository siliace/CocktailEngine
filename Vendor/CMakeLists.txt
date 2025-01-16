add_subdirectory(Catch2)
add_subdirectory(cmrc)
add_subdirectory(fmt)
add_subdirectory(SPIRV-Headers)

set(ENABLE_OPT OFF)
set(BUILD_EXTERNAL OFF)
set(BUILD_SHARED_LIBS OFF)
set(ENABLE_SPVREMAPPER OFF)
add_subdirectory(glslang)

add_subdirectory(magic_enum)

option(SPIRV_REFLECT_EXECUTABLE "" OFF)
option(SPIRV_REFLECT_EXAMPLES "" OFF)
option(SPIRV_REFLECT_STATIC_LIB "" ON)
add_subdirectory(SPIRV-Reflect)

add_subdirectory(stb_image)
add_subdirectory(stb_image_write)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(tinygltf)

add_subdirectory(tinyobjloader)
add_subdirectory(tinyxml2)

add_subdirectory(Vulkan-Headers)

set(VOLK_HEADERS_ONLY ON)
add_subdirectory(volk)
