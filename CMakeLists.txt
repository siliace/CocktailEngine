cmake_minimum_required(VERSION 3.20.5)
project(CocktailEngine)

if (WIN32)
	if (MSVC)
		add_definitions(/MP)
        add_compile_options(/utf-8)
	endif ()

	add_compile_definitions(UNICODE _UNICODE)
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Vendored static libraries get linked into Cocktail's shared libraries, so they
# must be position independent too (no-op on Windows)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(Cmake/macro.cmake)

add_subdirectory(Vendor)

# Enabled after Vendor so that third-party test suites are not registered with CTest
enable_testing()

add_subdirectory(Runtime)
add_subdirectory(Tests)
add_subdirectory(Samples)
