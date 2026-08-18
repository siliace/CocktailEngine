# -----------------------------------------------------------------------------
# ck_add_plugin - brings a plugin subproject in
#
# The name is declared here only. The plugin CMakeLists.txt merely declares its
# modules, which avoids two sources of truth drifting apart.
# TODO for a plugin living in its own repository, a ck_plugin() command guarded
# by a standalone-build check will be needed.
# -----------------------------------------------------------------------------

function(ck_add_plugin name)
    cmake_parse_arguments(ARG "" "DIRECTORY" "" ${ARGN})

    if(NOT ARG_DIRECTORY)
        _ck_state_get(plugins_dir CK_PLUGINS_DIR)
        if(NOT plugins_dir)
            message(FATAL_ERROR "ck_add_plugin: call ck_project() first.")
        endif()
        set(ARG_DIRECTORY "${plugins_dir}/${name}")
    endif()

    # Context handed down to the subdirectory scope, read by ck_add_module.
    set(CK_CURRENT_PLUGIN "${name}")
    set(CK_CURRENT_SOURCES_DIR "${ARG_DIRECTORY}/sources")
    set(CK_CURRENT_RESOURCES_DIR "${ARG_DIRECTORY}/resources")
    file(MAKE_DIRECTORY "${CK_CURRENT_SOURCES_DIR}" "${CK_CURRENT_RESOURCES_DIR}")
    # Created when it is not there, and left alone when it is: file(TOUCH) moves the
    # date of a file that already exists, and a CMakeLists.txt dated after the build
    # system it took part in generating is a build system CMake regenerates. Doing
    # that from the configure run itself is a reconfigure before every build, for
    # good, and every project file rewritten with it.
    _ck_declare_file("${ARG_DIRECTORY}/CMakeLists.txt")

    _ck_state_append(CK_PLUGINS "${name}")
    add_subdirectory("${ARG_DIRECTORY}" "${CMAKE_BINARY_DIR}/plugins/${name}")
endfunction()
