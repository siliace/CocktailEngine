project(Main)

cocktail_source_directory(MAIN_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Main/ExitCode.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/New.cpp
)

if (WIN32)
    cocktail_source_directory(MAIN_FILES    
        FOLDER "Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Win32/Win32Application.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Win32/Win32Application.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Win32/WinMain.cpp
    )
elseif (LINUX)
    cocktail_source_directory(MAIN_FILES
        FOLDER "Unix"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Unix/UnixApplication.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Unix/UnixApplication.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Main/Unix/UnixMain.cpp
    )
endif ()

cocktail_add_runtime_library(
    STATIC
    NAME Main
    SOURCES ${MAIN_FILES}
    DEPENDS Core
)
