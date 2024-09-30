project(HelloTriangle)

cocktail_source_directory(HELLO_TRIANGLE_FILES
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/HelloTriangle/main.cpp
)

cocktail_add_shader(HELLO_TRIANGLE_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert.spv
    TYPE vertex
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert
)

cocktail_add_shader(HELLO_TRIANGLE_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag.spv
    TYPE fragment
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag
)

cocktail_resource_directory(HELLO_TRIANGLE_FILES
    FOLDER "shaders"
    RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag
)

cocktail_add_sample(
    NAME 
        hello-triangle
    DEPENDS 
        Core Main Graphic Renderer Vulkan
    SOURCES 
        ${HELLO_TRIANGLE_FILES}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag.spv
)