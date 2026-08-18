project(stb_image)

add_library(
    stb_image 
    STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image/stb_image.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image/stb_image.cpp
)

target_include_directories(stb_image 
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)
