add_subdirectory(Catch2)
add_subdirectory(cmrc)
add_subdirectory(fmt)
add_subdirectory(SPIRV-Headers)

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
add_subdirectory(tinyobjloader)
add_subdirectory(tinyxml2)

if (WIN32)
    set(VOLK_STATIC_DEFINES "VK_USE_PLATFORM_WIN32_KHR")
endif ()
add_subdirectory(volk)
