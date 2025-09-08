macro(cocktail_source_directory THIS_FILES)
    # parse the arguments
    cmake_parse_arguments(THIS "" "FOLDER" "INCLUDE;SRC" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_source_directory: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    list(APPEND ${THIS_FILES} ${THIS_INCLUDE} ${THIS_SRC})
    
    source_group("include/${THIS_FOLDER}" FILES ${THIS_INCLUDE})
    source_group("src/${THIS_FOLDER}" FILES ${THIS_SRC})
endmacro()

macro(cocktail_resource_directory THIS_FILES)
    # parse the arguments
    cmake_parse_arguments(THIS "" "FOLDER" "RESOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_resource_directory: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    list(APPEND ${THIS_FILES} ${THIS_RESOURCES})
    
    source_group("resources/${THIS_FOLDER}" FILES ${THIS_RESOURCES})
endmacro()

function(cocktail_add_tool)
    # parse the arguments
    cmake_parse_arguments(THIS "" "NAME" "SOURCES;DEPENDS;LINK;RESOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_tool: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()
    
    # create the target
    string(TOLOWER cocktail-${THIS_NAME} THIS_TARGET)
    add_executable(${THIS_TARGET} ${THIS_SOURCES})

    # enable C++17 support
    target_compile_features(${THIS_TARGET} PUBLIC cxx_std_17)

    # set the target's folder (for Visual Studio)
    set_target_properties(${THIS_TARGET} PROPERTIES FOLDER "Tools")

    # set build macro and NOMINMAX
    target_compile_definitions(${THIS_TARGET} PUBLIC NOMINMAX)

    # set includes directories
    target_include_directories(${THIS_TARGET}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/src
    )

    # define COCKTAIL_STATIC if the build type is not set to 'shared'
    if (NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${THIS_TARGET} PUBLIC "COCKTAIL_STATIC")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${THIS_TARGET} PRIVATE -Wno-changes-meaning)
    endif ()

    foreach(DEPENDENCY IN LISTS THIS_DEPENDS)
        set(COCKTAIL_DEPENDENCY_NAME "COCKTAIL::${DEPENDENCY}")
        if (TARGET ${COCKTAIL_DEPENDENCY_NAME})
            message("Link ${THIS_NAME} against ${COCKTAIL_DEPENDENCY_NAME}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${COCKTAIL_DEPENDENCY_NAME})
        else ()
            message("Link ${THIS_NAME} against ${DEPENDENCY}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${DEPENDENCY})
        endif ()
    endforeach()

    target_link_libraries(${THIS_TARGET} PRIVATE ${THIS_LINK})

    if (WIN32)
        add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
            COMMAND_EXPAND_LISTS
        )
        
        if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
            add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
                COMMAND_EXPAND_LISTS
            )
        endif ()
    endif ()

    if (THIS_DEBUG_DIRECTORY AND CMAKE_GENERATOR MATCHES "Visual Studio")
        set_target_properties(${THIS_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${THIS_DEBUG_DIRECTORY})
    endif ()

    if (THIS_RESOURCES)
        string(TOLOWER ${THIS_NAME} THIS_RESOURCES_PREFIX)
        string(REPLACE "-" "_" THIS_RESOURCES_PREFIX ${THIS_RESOURCES_PREFIX})
        cmrc_add_resource_library(rc_${THIS_TARGET} NAMESPACE "${THIS_RESOURCES_PREFIX}" PREFIX "${THIS_RESOURCES_PREFIX}" ${THIS_RESOURCES})
        target_link_libraries(${THIS_TARGET} PRIVATE rc_${THIS_TARGET})
    endif ()
endfunction()

function(cocktail_add_sample)
    # parse the arguments
    cmake_parse_arguments(THIS "" "NAME;DEBUG_DIRECTORY" "SOURCES;DEPENDS;RESOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_sample: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()
    
    # create the target
    string(TOLOWER cocktail-sample-${THIS_NAME} THIS_TARGET)
    add_executable(${THIS_TARGET} ${THIS_SOURCES})

    # enable C++17 support
    target_compile_features(${THIS_TARGET} PUBLIC cxx_std_17)

    # set the target's folder (for Visual Studio)
    set_target_properties(${THIS_TARGET} PROPERTIES FOLDER "Samples")

    # set build macro and NOMINMAX
    target_compile_definitions(${THIS_TARGET} PUBLIC NOMINMAX)

    # set includes directories
    target_include_directories(${THIS_TARGET}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/src
    )

    # define COCKTAIL_STATIC if the build type is not set to 'shared'
    if (NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${THIS_TARGET} PUBLIC "COCKTAIL_STATIC")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${THIS_TARGET} PRIVATE -Wno-changes-meaning)
    endif ()

    foreach(dependency IN LISTS THIS_DEPENDS)
        target_link_libraries(${THIS_TARGET} PUBLIC COCKTAIL::${dependency})
    endforeach()

    if (WIN32)
        add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
            COMMENT "Copying $<TARGET_RUNTIME_DLLS:${THIS_TARGET}> to $<TARGET_FILE_DIR:${THIS_TARGET}>"
            COMMAND_EXPAND_LISTS
        )

        if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
            add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
                COMMENT "Copying $<TARGET_PDB_FILE:${THIS_TARGET}> to $<TARGET_FILE_DIR:${THIS_TARGET}>"
                COMMAND_EXPAND_LISTS
            )
        endif ()
    endif ()

    if (THIS_DEBUG_DIRECTORY AND CMAKE_GENERATOR MATCHES "Visual Studio")
        target_sources(${THIS_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/.natvis")
        set_target_properties(${THIS_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${THIS_DEBUG_DIRECTORY})
    endif ()

    if (THIS_RESOURCES)
        string(TOLOWER ${THIS_NAME} THIS_RESOURCES_PREFIX)
        string(REPLACE "-" "_" THIS_RESOURCES_PREFIX ${THIS_RESOURCES_PREFIX})
        cmrc_add_resource_library(rc_${THIS_TARGET} NAMESPACE "${THIS_RESOURCES_PREFIX}" PREFIX "${THIS_RESOURCES_PREFIX}" ${THIS_RESOURCES})
        target_link_libraries(${THIS_TARGET} PRIVATE rc_${THIS_TARGET})
    endif ()
endfunction()

function(cocktail_add_runtime_library)
    # parse the arguments
    cmake_parse_arguments(THIS "STATIC" "NAME" "DEPENDS;LINK;SOURCES;DEFINES;RESOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_runtime_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    # create the target
    string(TOLOWER cocktail-runtime-${THIS_NAME} THIS_TARGET)
    if (THIS_STATIC)
        add_library(${THIS_TARGET} STATIC ${THIS_SOURCES})
    else()
        add_library(${THIS_TARGET} ${THIS_SOURCES})

        set_target_properties(${THIS_TARGET}
            PROPERTIES
                C_VISIBILITY_PRESET hidden
                CXX_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON
            )
    endif ()
    add_library(COCKTAIL::${THIS_NAME} ALIAS ${THIS_TARGET})

    # enable C++17 support
    target_compile_features(${THIS_TARGET} PUBLIC cxx_std_17)

    # set build macro and NOMINMAX
    string(TOUPPER COCKTAIL_${THIS_NAME}_BUILD THIS_BUILD_MACRO)
    list(APPEND THIS_DEFINES ${THIS_BUILD_MACRO})

    target_compile_definitions(${THIS_TARGET}
        PUBLIC NOMINMAX
        PRIVATE ${THIS_DEFINES}
    )

    if (WIN32)
        target_compile_definitions(${THIS_TARGET}
            PUBLIC
                UNICODE
                _UNICODE
        )
    endif ()

    # set includes directories
    target_include_directories(${THIS_TARGET}
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PRIVATE ${PROJECT_SOURCE_DIR}/src
    )

    # set the target's folder (for Visual Studio)
    set_target_properties(${THIS_TARGET} PROPERTIES FOLDER "Runtime")

    # define COCKTAIL_STATIC if the build type is not set to 'shared'
    if (NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${THIS_TARGET} PUBLIC "COCKTAIL_STATIC")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${THIS_TARGET} PRIVATE -Wno-changes-meaning)
    endif ()

    foreach(DEPENDENCY IN LISTS THIS_DEPENDS)
        set(COCKTAIL_DEPENDENCY_NAME "COCKTAIL::${DEPENDENCY}")
        if (TARGET ${COCKTAIL_DEPENDENCY_NAME})
            message("Link ${THIS_NAME} against ${COCKTAIL_DEPENDENCY_NAME}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${COCKTAIL_DEPENDENCY_NAME})
        else ()
            message("Link ${THIS_NAME} against ${DEPENDENCY}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${DEPENDENCY})
        endif ()
    endforeach()

    target_link_libraries(${THIS_TARGET} PRIVATE ${THIS_LINK})

    if (THIS_RESOURCES)
        string(TOLOWER ${THIS_NAME} THIS_RESOURCES_PREFIX)
        string(REPLACE "-" "_" THIS_RESOURCES_PREFIX ${THIS_RESOURCES_PREFIX})
        cmrc_add_resource_library(rc_${THIS_TARGET} NAMESPACE "${THIS_NAME}" PREFIX "${THIS_RESOURCES_PREFIX}" ${THIS_RESOURCES})
        target_link_libraries(${THIS_TARGET} PRIVATE rc_${THIS_TARGET})
    endif ()
endfunction()

function(cocktail_add_catch_test)
    # parse the arguments
    cmake_parse_arguments(THIS "" "NAME" "DEPENDS;SOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_catch_test: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    string(TOLOWER cocktail-test-${THIS_NAME} THIS_TARGET)
    add_executable(${THIS_TARGET} ${THIS_SOURCES})

    # enable C++17 support
    target_compile_features(${THIS_TARGET} PUBLIC cxx_std_17)
    
    target_compile_definitions(${THIS_TARGET}
        PUBLIC NOMINMAX
    )

    # set includes directories
    target_include_directories(${THIS_TARGET}
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PRIVATE ${PROJECT_SOURCE_DIR}/src
    )

    # set the target's folder (for Visual Studio)
    set_target_properties(${THIS_TARGET} PROPERTIES FOLDER "Tests")

    foreach(DEPENDENCY IN LISTS THIS_DEPENDS)
        set(COCKTAIL_DEPENDENCY_NAME "COCKTAIL::${DEPENDENCY}")
        if (TARGET ${COCKTAIL_DEPENDENCY_NAME})
            message("Link ${THIS_NAME} against ${COCKTAIL_DEPENDENCY_NAME}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${COCKTAIL_DEPENDENCY_NAME})
        else ()
            message("Link ${THIS_NAME} against ${DEPENDENCY}")
            target_link_libraries(${THIS_TARGET} PUBLIC ${DEPENDENCY})
        endif ()
    endforeach()

    target_link_libraries(${THIS_TARGET} PRIVATE ${THIS_LINK} Catch2::Catch2WithMain)

    if (WIN32)
        add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
            COMMENT "Copying $<TARGET_RUNTIME_DLLS:${THIS_TARGET}> to $<TARGET_FILE_DIR:${THIS_TARGET}>"
            COMMAND_EXPAND_LISTS
        )

        if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
            add_custom_command(TARGET ${THIS_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${THIS_TARGET}> $<TARGET_FILE_DIR:${THIS_TARGET}>
                COMMENT "Copying $<TARGET_PDB_FILE:${THIS_TARGET}> to $<TARGET_FILE_DIR:${THIS_TARGET}>"
                COMMAND_EXPAND_LISTS
            )
        endif ()
    endif ()
endfunction()

macro(cocktail_add_shader THIS_FILES)
    # parse the arguments
    cmake_parse_arguments(THIS "" "OUTPUT;TYPE;FILE" "MACRO" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling cocktail_add_shader: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    file(TOUCH ${THIS_OUTPUT})
        
    set(MACRO_STR "")
    foreach (MACRO IN LISTS THIS_MACRO)
        set(MACRO_STR ${MACRO_STR} -D${MACRO})
    endforeach ()
    string(STRIP "${MACRO_STR}" MACRO_STR)

    add_custom_command(
        OUTPUT ${THIS_OUTPUT}
        COMMAND $<TARGET_FILE:glslang-standalone> ${THIS_FILE} -o ${THIS_OUTPUT} -V100 -S ${THIS_TYPE} ${MACRO_STR}
        COMMENT "Compiling ${THIS_FILE} to SPIR-V ${THIS_OUTPUT}"
        DEPENDS ${THIS_FILE} glslang-standalone
        VERBATIM
    )
endmacro()