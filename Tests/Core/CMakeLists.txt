project(CoreTest)

cocktail_source_directory(CORE_TEST_FILES
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Array.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/String.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Application"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/ServiceContainer.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Math"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Angle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Plane.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Math/Vector"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector/Vector4.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Math/Volume"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Volume/Box.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Volume/Cylinder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Volume/Sphere.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Signal"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Signal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Observable.cpp
)

cocktail_source_directory(CORE_TEST_FILES    
    FOLDER "System/FileSystem"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/System/FileSystem/Path.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/System/FileSystem/URI.cpp
)

cocktail_source_directory(CORE_TEST_FILES    
    FOLDER "System/Network/Address"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/System/Network/Address/IpAddressV4.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Utility"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/Any.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ByteArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ByteArrayView.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/Optional.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/TranslatorCast.cpp
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
    DEPENDS Core Main
    SOURCES ${CORE_TEST_FILES}
    CUSTOM_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp
)