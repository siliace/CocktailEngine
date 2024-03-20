add_subdirectory(Catch2)
add_subdirectory(cmrc)
add_subdirectory(fmt)
add_subdirectory(magic_enum)

option(SPIRV_REFLECT_EXECUTABLE "" OFF)
option(SPIRV_REFLECT_EXAMPLES "" OFF)
option(SPIRV_REFLECT_STATIC_LIB "" ON)
add_subdirectory(SPIRV-Reflect)

add_subdirectory(stb_image)

if (WIN32)
    set (VOLK_STATIC_DEFINES "VK_USE_PLATFORM_WIN32_KHR")
endif ()

add_subdirectory(tinyxml2)
add_subdirectory(volk)
