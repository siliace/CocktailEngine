# -----------------------------------------------------------------------------
# ck_project - root of a game project
#
# Deliberately does not call project(): CMake requires a literal, direct call in
# the top-level CMakeLists.txt and warns when the call comes from a macro or an
# included file. project() therefore stays explicit on the user side, and NAME is
# only checked against PROJECT_NAME.
#
# All state goes into GLOBAL properties, which lets this command be a function
# (isolated scope) while remaining readable from plugin directory scopes.
# -----------------------------------------------------------------------------

function(ck_project)
    cmake_parse_arguments(ARG "AGGREGATE_TARGET" "NAME" "" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ck_project: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT PROJECT_NAME)
        message(FATAL_ERROR "ck_project: call project() before ck_project().")
    endif()
    if(NOT ARG_NAME)
        set(ARG_NAME "${PROJECT_NAME}")
    elseif(NOT ARG_NAME STREQUAL PROJECT_NAME)
        message(FATAL_ERROR
            "ck_project: NAME '${ARG_NAME}' does not match project() '${PROJECT_NAME}'.")
    endif()
    _ck_state_get(existing_project CK_PROJECT_NAME)
    if(existing_project)
        message(FATAL_ERROR "ck_project: already called for '${existing_project}'.")
    endif()

    set(project_root "${CMAKE_CURRENT_SOURCE_DIR}")

    # Outputs default to the source tree, following the project convention.
    # Overridable to keep the tree clean or to support multi-config builds.
    set(CK_INTERMEDIATE_DIR "${project_root}/intermediate"
        CACHE PATH "Directory holding assets in engine format")
    set(CK_OUTPUT_DIR "${project_root}/bin"
        CACHE PATH "Directory holding binaries and packaged assets")
    set(CK_BUILD_DIR "${project_root}/build"
        CACHE PATH "Directory holding build files")

    # Whether this project ships what it builds. A cache entry, and off when the
    # project is not the one being built: a game consuming the engine has no reason
    # to re-install the modules it links, and installing them a second time under
    # its own package name would give the same module two ways of being found.
    set(CK_INSTALL "${PROJECT_IS_TOP_LEVEL}" CACHE BOOL
        "Generate install rules for the modules of this project")

    # Where an install lands, under whatever prefix cmake --install is given. The
    # platform conventions come from GNUInstallDirs rather than from names spelled
    # here, so a distribution that puts libraries in lib64 gets them there.
    include(GNUInstallDirs)

    _ck_state_set(CK_INSTALL_ENABLED "${CK_INSTALL}")
    _ck_state_set(CK_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    _ck_state_set(CK_INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}")
    _ck_state_set(CK_INSTALL_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
    # The package the consumer finds, named after the project: lib/cmake/<Project>.
    _ck_state_set(CK_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${ARG_NAME}")
    # Debugger visualisers are neither headers nor binaries, and a linker is given
    # their path rather than reading them from a known place.
    _ck_state_set(CK_INSTALL_DEBUG_DIR "${CMAKE_INSTALL_DATADIR}/${ARG_NAME}/debug")

    # Which platform directories every module compiles. Defaults to what the system
    # provides, and is a cache entry because choosing a backend is a build decision:
    # swapping Xlib for another one is this list, not a module to touch.
    _ck_default_platform_tags(default_platform_tags)
    set(CK_PLATFORM_TAGS "${default_platform_tags}" CACHE STRING
        "Platform tags whose directories take part in the build")

    # Whether the modules build their tests. A cache entry rather than something a
    # module decides: tests are built when someone is about to run them, which is a
    # build decision like choosing a backend. Off when this project is not the one
    # being built, since a game has no reason to build the tests of the engine it
    # links, nor to configure the test framework they would need.
    set(CK_BUILD_TESTS "${PROJECT_IS_TOP_LEVEL}" CACHE BOOL
        "Build the test binary of every module that has tests")

    # Whether the code of this project is built under the address sanitizer. A cache
    # entry like the two above, and for the same reason: it is a build decision, not
    # something a module gets to make on its own.
    #
    # Spelled without the CK_ prefix the rest of the framework uses, since that is
    # the name the command line asks for it by:
    #
    #   cmake -B build -DENABLE_ASAN=ON
    option(ENABLE_ASAN
        "Build the modules, games and tests with the address sanitizer" OFF)

    # The flags CMake writes for the configurations hold two things the MSVC
    # sanitizer refuses outright, so they are taken out of the debug configurations
    # before a single target is created. Published into the directory scope this was
    # called from, which every module and game is declared under, so what the cache
    # holds stays what the user asked for: turning the option off puts the runtime
    # checks and the incremental link straight back.
    if(ENABLE_ASAN AND MSVC)
        foreach(variable IN ITEMS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_EXE_LINKER_FLAGS_DEBUG
                CMAKE_SHARED_LINKER_FLAGS_DEBUG
                CMAKE_MODULE_LINKER_FLAGS_DEBUG
                CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
                CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
                CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO)
            _ck_sanitizer_strip_conflicts(adjusted "${${variable}}")
            set(${variable} "${adjusted}" PARENT_SCOPE)
        endforeach()
    endif()

    # What the instrumented half of the build and the half that is not have to agree
    # on to end up in one binary. A directory property rather than something handed to
    # a target, so that it reaches the vendored libraries and the tools added below
    # this point as well as the code of the project: it is a decision of the build,
    # and an object left out of it is an object the linker refuses.
    if(ENABLE_ASAN)
        _ck_sanitizer_uniform_definitions(sanitizer_definitions)
        if(sanitizer_definitions)
            add_compile_definitions(${sanitizer_definitions})
        endif()
    endif()

    # Editors and language servers resolve includes from the compilation database,
    # and a good part of what a module includes is generated into the build tree.
    # Without it, an editor reports the generated base of every module as missing
    # while the build is perfectly happy. On by default, then, and left alone when
    # the user has an opinion about it. CMake seeds the entry itself, empty, so a
    # plain set(... CACHE ...) would never take.
    if(NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS
            OR CMAKE_EXPORT_COMPILE_COMMANDS STREQUAL "")
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL
            "Write compile_commands.json for tooling" FORCE)
    endif()

    file(MAKE_DIRECTORY "${project_root}/sources")

    _ck_keep_directory("${project_root}/plugins")
    _ck_keep_directory("${project_root}/resources")
    _ck_keep_directory("${project_root}/thirdparty")

    _ck_ignore_directory("${CK_BUILD_DIR}")
    _ck_ignore_directory("${CK_OUTPUT_DIR}")
    _ck_ignore_directory("${CK_INTERMEDIATE_DIR}")

    # CTest only walks a tree whose root has testing enabled, and the root is the
    # directory this is called from. enable_testing() marks the scope it is called
    # in, though, and a function is a scope of its own, so what it sets would be
    # forgotten on return: it is lifted into the caller, which is that directory.
    # Modules are added from there, so they inherit it and their tests are reached.
    if(CK_BUILD_TESTS)
        enable_testing()
        set(CMAKE_TESTING_ENABLED "${CMAKE_TESTING_ENABLED}" PARENT_SCOPE)
    endif()

    _ck_state_set(CK_PROJECT_NAME "${ARG_NAME}")
    # Empty when project() was called without VERSION, which is legal: the package
    # then ships no version file, and find_package is asked for no version either.
    _ck_state_set(CK_PROJECT_VERSION "${PROJECT_VERSION}")
    _ck_state_set(CK_PROJECT_GENERATE_AGGREGATE "${ARG_AGGREGATE_TARGET}")
    _ck_state_set(CK_PROJECT_ROOT "${project_root}")
    _ck_state_set(CK_SOURCES_DIR "${project_root}/sources")
    _ck_state_set(CK_RESOURCES_DIR "${project_root}/resources")
    _ck_state_set(CK_PLUGINS_DIR "${project_root}/plugins")
    _ck_state_set(CK_THIRDPARTY_DIR "${project_root}/thirdparty")
    _ck_state_set(CK_INTERMEDIATE_DIR "${CK_INTERMEDIATE_DIR}")
    _ck_state_set(CK_OUTPUT_DIR "${CK_OUTPUT_DIR}")

    # Declared before anything else, so that a module joining it and a game linking
    # it never have to wonder whether it is there yet.
    if (ARG_AGGREGATE_TARGET)
        _ck_declare_project_aggregate()
    endif()

    # Targets are created eagerly, but everything that needs a global view
    # (asset cooking, manifest, validation) waits until configuration ends.
    # The user has nothing to write for this to happen.
    cmake_language(DEFER CALL ck_finalize)
endfunction()
