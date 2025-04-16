cmake_minimum_required(VERSION 3.20.5)
project(CocktailEngine)

if(MSVC)
	add_definitions(/MP)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(Cmake/macro.cmake)

add_subdirectory(Vendor)

add_subdirectory(Runtime)
add_subdirectory(Tests)
add_subdirectory(Samples)
