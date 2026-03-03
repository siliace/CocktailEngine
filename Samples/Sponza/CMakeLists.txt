project(Sponza)

cocktail_source_directory(SPONZA_FILES
        FOLDER "ImUi"
        INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/ImUi.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/Menu/MenuBarManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/Menu/MenuModule.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/Menu/PanelControllerMenuModule.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/Panel/Panel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/Panel/PanelManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/WindowContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/WindowRenderer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/ImUi/WindowResources.hpp
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/ImUi.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/Menu/MenuBarManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/Menu/MenuModule.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/Menu/PanelControllerMenuModule.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/Panel/Panel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/Panel/PanelManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/WindowContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/WindowRenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/ImUi/WindowResources.cpp
)

cocktail_source_directory(SPONZA_FILES
        INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/SceneMenuModule.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Sponza/SystemMenuModule.hpp
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/New.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/SceneMenuModule.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/SystemMenuModule.cpp
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
    LINK ImGui
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders/ImGui/default.frag.spv
)
