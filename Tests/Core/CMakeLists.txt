project(CoreTest)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Application"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/ServiceContainer.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Math/Vector"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector4.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Memory"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory/Ref.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Utility/PropertyTree"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/PropertyTree/PropertyTreeValue.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Utility/Time"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/Time/Duration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/Time/TimeUnit.cpp
)

cocktail_add_catch_test(
    NAME Core
    DEPENDS Core
    SOURCES ${CORE_TEST_FILES}
)