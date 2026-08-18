# -----------------------------------------------------------------------------
# ck_declare_thirdparty - vendors a library into the module that needs it
#
# Called from the CMakeLists.txt of a module, for a directory sitting in its
# thirdparty/:
#
#   thirdparty/MagicEnum/CMakeLists.txt   how the library is brought in
#   thirdparty/MagicEnum/magic_enum/      the library itself, usually a submodule
#
# The library keeps its own build. That intermediate CMakeLists.txt is where its
# options are set and its add_subdirectory happens, so upgrading the submodule
# never touches the module, and the framework never has to know what the library
# calls its targets or how it wants to be configured.
#
# TARGETS names what the module links, since only the library decides what its
# targets are called. Visibility defaults to PRIVATE, which is what vendoring
# usually means: the library is an implementation detail of the module, and a
# consumer has no business seeing it. PUBLIC is for a library that shows up in a
# published header, and therefore in the API of the module: whoever includes that
# header has to be able to compile it.
#
# Where the library is read from follows the scope of the call, the way the
# namespace of a module does:
#
#   from the CMakeLists.txt of a module   <module>/thirdparty/<Name>, linked into it
#   from the project CMakeLists.txt       <project>/thirdparty/<Name>, linked into nothing
#
# A library declared at project level is one no single module owns, so there is
# nothing for it to be linked into and no visibility to give it: it is brought in
# once, and what needs it says so.
# -----------------------------------------------------------------------------

function(ck_declare_thirdparty name)
    cmake_parse_arguments(ARG "PUBLIC;PRIVATE" "" "TARGETS" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "ck_declare_thirdparty(${name}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(ARG_PUBLIC AND ARG_PRIVATE)
        message(FATAL_ERROR
            "ck_declare_thirdparty(${name}): PUBLIC and PRIVATE are exclusive.")
    endif()

    # A vendored library belongs to the module that needs it, and failing that to the
    # project. What it cannot belong to is a plugin: a plugin holds modules, and a
    # library vendored beside them would have no one to answer for it.
    if(CK_CURRENT_MODULE)
        set(owner "${CK_CURRENT_MODULE}")
        set(directory "${CK_CURRENT_MODULE_DIR}/thirdparty/${name}")
    else()
        if(CK_CURRENT_PLUGIN)
            message(FATAL_ERROR
                "ck_declare_thirdparty(${name}): plugin '${CK_CURRENT_PLUGIN}' cannot vendor "
                "a library of its own. Vendor it in the module that needs it.")
        endif()

        _ck_state_get(project_name CK_PROJECT_NAME)
        _ck_state_get(thirdparty_dir CK_THIRDPARTY_DIR)
        if(NOT thirdparty_dir)
            message(FATAL_ERROR "ck_declare_thirdparty(${name}): call ck_project() first.")
        endif()
        if(ARG_PUBLIC OR ARG_PRIVATE)
            message(FATAL_ERROR
                "ck_declare_thirdparty(${name}): PUBLIC and PRIVATE say what a module "
                "publishes of a library it vendors. At project level there is nothing to "
                "link it into, so neither applies.")
        endif()

        set(owner "${project_name}")
        set(directory "${thirdparty_dir}/${name}")
    endif()

    if(NOT EXISTS "${directory}/CMakeLists.txt")
        message(FATAL_ERROR
            "ck_declare_thirdparty(${name}): no CMakeLists.txt in ${directory}\n"
            "A vendored library is a directory in thirdparty/ holding the library and a "
            "CMakeLists.txt bringing it in. If the library is a submodule, it may simply "
            "not be checked out: git submodule update --init --recursive")
    endif()

    # The same library vendored twice would have its targets created twice, and CMake
    # refuses that with an error naming the target rather than the library. Caught
    # here, while the two owners involved can still be named.
    _ck_state_get(previous_owner CK_THIRDPARTY_${name}_OWNER)
    if(previous_owner)
        message(FATAL_ERROR
            "ck_declare_thirdparty(${name}): already vendored by ${previous_owner}, and it "
            "cannot be brought in twice: its targets would collide. Vendor it once, and have "
            "whoever else needs it depend on that.")
    endif()

    # Modules are shared libraries, so whatever static library ends up inside one has
    # to be position independent, which a third-party build has no reason to know.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # EXCLUDE_FROM_ALL leaves out the examples, tests and tools the library ships:
    # what the module links is built as a dependency of the module, the rest is not
    # this project's business.
    add_subdirectory("${directory}" "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/${name}"
        EXCLUDE_FROM_ALL)

    foreach(thirdparty_target IN LISTS ARG_TARGETS)
        if(NOT TARGET ${thirdparty_target})
            message(FATAL_ERROR
                "ck_declare_thirdparty(${name}): '${thirdparty_target}' is not a target once "
                "${directory} has been processed. Check the name the library gives it.")
        endif()

        # Grouped out of the way in the IDE, under the real target: properties cannot
        # be set on an alias, and an interface library has nothing to group.
        get_target_property(aliased ${thirdparty_target} ALIASED_TARGET)
        if(aliased)
            set(real_target "${aliased}")
        else()
            set(real_target "${thirdparty_target}")
        endif()

        _ck_configure_target(${real_target})

        get_target_property(type ${real_target} TYPE)
        if(NOT type STREQUAL "INTERFACE_LIBRARY")
            set_target_properties(${real_target} PROPERTIES FOLDER "ThirdParty")
        endif()
    endforeach()

    # Only a module has something to link the library into, and something to publish
    # it through.
    if(CK_CURRENT_MODULE AND ARG_TARGETS)
        if(ARG_PUBLIC)
            set(visibility PUBLIC)
        else()
            set(visibility PRIVATE)
        endif()
        target_link_libraries(${CK_CURRENT_MODULE_TARGET} ${visibility} ${ARG_TARGETS})
    endif()

    _ck_state_append(CK_THIRDPARTIES "${name}")
    _ck_state_set(CK_THIRDPARTY_${name}_OWNER "${owner}")
    _ck_state_set(CK_THIRDPARTY_${name}_TARGETS "${ARG_TARGETS}")
    # Whether the library shows up in the API of the module, which is what decides
    # whether its headers are part of what an install ships.
    if(ARG_PUBLIC)
        _ck_state_set(CK_THIRDPARTY_${name}_VISIBILITY PUBLIC)
    else()
        _ck_state_set(CK_THIRDPARTY_${name}_VISIBILITY PRIVATE)
    endif()
    if(CK_CURRENT_MODULE)
        _ck_state_set(CK_THIRDPARTY_${name}_MODULE "${CK_CURRENT_MODULE}")
        _ck_state_append(CK_THIRDPARTIES_${CK_CURRENT_MODULE} "${name}")
    endif()
endfunction()
