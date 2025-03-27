project(CoreTest)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Application"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/ServiceContainer.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Math"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Math/Angle.cpp
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
    FOLDER "Signal"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Signal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Signal/Observable.cpp
)

cocktail_source_directory(CORE_TEST_FILES    
    FOLDER "System/Network/Address"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/System/Network/Address/IpAddressV4.cpp
)

cocktail_source_directory(CORE_TEST_FILES
    FOLDER "Utility"    
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ByteArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/ByteArrayView.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/Optional.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility/StringUtils.cpp
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
    DEPENDS Core
    SOURCES ${CORE_TEST_FILES}
)