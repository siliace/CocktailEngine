project(stb_image)

cocktail_source_directory(STB_IMAGE_FILES
    INCLUDE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image/stb_image.h
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image/stb_image.cpp
)

add_library(stb_image STATIC ${STB_IMAGE_FILES})
target_include_directories(stb_image 
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)
