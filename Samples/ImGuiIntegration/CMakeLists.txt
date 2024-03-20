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

cocktail_add_shader(IMGUI_INTEGRATION_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.vert.spv
    TYPE vertex
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.vert
)

cocktail_add_shader(IMGUI_INTEGRATION_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.frag.spv
    TYPE fragment
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.frag
)

cocktail_source_directory(IMGUI_INTEGRATION_FILES
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/main.cpp
)


cocktail_source_directory(IMGUI_INTEGRATION_FILES
    FOLDER "ImGui"
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ImGuiIntegration/ImGui/imstb_truetype.h
)
cocktail_resource_directory(IMGUI_INTEGRATION_FILES
    FOLDER "shaders"
    RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.frag.spv
)

cocktail_add_sample(
    NAME ImGui-Integration
    SOURCES ${IMGUI_INTEGRATION_FILES}
    DEPENDS Main
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
)