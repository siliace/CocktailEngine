cmake_minimum_required(VERSION 3.20.5)
project(CocktailEngine)

set(CMAKE_MSVC_PARALLEL_ENABLE ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(Cmake/macro.cmake)

add_subdirectory(Vendor)

add_subdirectory(Runtime)
add_subdirectory(Tools)
add_subdirectory(Tests)
add_subdirectory(Samples)
