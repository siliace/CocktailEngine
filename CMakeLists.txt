cmake_minimum_required(VERSION 3.20.5)
project(CocktailEngine)

if (WIN32)
	if (MSVC)
		add_definitions(/MP)
	endif ()

	add_compile_definitions(UNICODE _UNICODE)
	add_compile_options(/utf-8)
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(Cmake/macro.cmake)

add_subdirectory(Vendor)

add_subdirectory(Runtime)
add_subdirectory(Tests)
add_subdirectory(Samples)
