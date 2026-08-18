project(stb_image_write)

add_library(
    stb_image_write 
    STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image_write/stb_image_write.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image_write/stb_image_write.cpp
)

target_include_directories(stb_image_write 
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)
