project(stb_image_write)

cocktail_source_directory(STB_IMAGE_WRITE_FILES
    INCLUDE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image_write/stb_image_write.h
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image_write/stb_image_write.cpp
)

add_library(stb_image_write STATIC ${STB_IMAGE_WRITE_FILES})
target_include_directories(stb_image_write
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)
