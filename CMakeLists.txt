cmake_minimum_required(VERSION 3.20.5)
project(CocktailEngine)

if(CMAKE_MSVC_PARALLEL)
  if(CMAKE_MSVC_PARALLEL GREATER 0)
    add_definitions(/MP${CMAKE_MSVC_PARALLEL})
  else()
    add_definitions(/MP)
  endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(Cmake/macro.cmake)

add_subdirectory(Vendor)

add_subdirectory(Runtime)
add_subdirectory(Tests)
add_subdirectory(Samples)
