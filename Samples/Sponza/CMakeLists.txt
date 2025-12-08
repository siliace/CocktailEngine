project(Sponza)

cocktail_source_directory(SPONZA_FILES
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGuiOverlay.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGuiOverlay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGuiServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGuiServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/New.cpp
)

cocktail_source_directory(SPONZA_FILES
    FOLDER "ImGui"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImGui/imstb_truetype.h
)

cocktail_add_shader(SPONZA_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.vert.spv
    TYPE vert
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.vert
)

cocktail_add_shader(SPONZA_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.frag.spv
    TYPE frag
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.frag
)

cocktail_resource_directory(SPONZA_FILES
    RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.frag
)

cocktail_add_sample(
    NAME sponza
    SOURCES ${SPONZA_FILES}
    DEPENDS Core Main Graphic Renderer Vulkan
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.frag.spv
)
