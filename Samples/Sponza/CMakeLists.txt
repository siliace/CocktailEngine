project(Sponza)

cocktail_source_directory(SPONZA_FILES
    SRC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Sponza/main.cpp
)

cocktail_add_sample(
    NAME sponza
    SOURCES ${SPONZA_FILES}
    DEPENDS Core Main Graphic Renderer Vulkan
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
)
