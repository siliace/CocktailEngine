project(ImGui)

cocktail_source_directory(IMGUI_FILES
        SRC
        imconfig.h
        imgui.h
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
)

add_library(ImGui STATIC ${IMGUI_FILES})
target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR})
