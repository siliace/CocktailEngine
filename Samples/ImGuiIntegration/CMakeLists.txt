project(HelloTriangle)

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
)

cocktail_add_sample(
    NAME 
        ImGui-Integration
    DEPENDS 
        Core Main Graphic Renderer Vulkan
    SOURCES 
        ${IMGUI_INTEGRATION_FILES}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/imgui.frag.spv
)