project(HelloTriangle)

macro(cocktail_add_shader THIS_FILES)
    # parse the arguments
    cmake_parse_arguments(THIS "" "OUTPUT;TYPE;FILE" "" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_shader: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    file(TOUCH ${THIS_OUTPUT})

    add_custom_command(
        OUTPUT ${THIS_OUTPUT}
        COMMAND glslc ${THIS_FILE} -o ${THIS_OUTPUT} -fshader-stage=${THIS_TYPE}
        DEPENDS ${THIS_FILE}
        VERBATIM
    )
endmacro()

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

cocktail_source_directory(HELLO_TRIANGLE_FILES
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/HelloTriangle/main.cpp
)

cocktail_resource_directory(HELLO_TRIANGLE_FILES
    FOLDER "shaders"
    RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/default.frag.spv
)

cocktail_add_sample(
    NAME hello-triangle
    SOURCES ${HELLO_TRIANGLE_FILES}
    DEPENDS Main
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
)