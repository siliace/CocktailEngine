project(FrustumCulling)

cocktail_source_directory(FRUSTUM_CULLING_FILES
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/FrustumCulling/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/FrustumCulling/New.cpp
)

cocktail_add_sample(
    NAME frustum-culling
    SOURCES ${FRUSTUM_CULLING_FILES}
    DEPENDS Core Main Graphic Renderer Vulkan
    DEBUG_DIRECTORY ${PROJECT_SOURCE_DIR}
)
