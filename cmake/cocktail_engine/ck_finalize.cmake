# -----------------------------------------------------------------------------
# ck_finalize - called automatically at the end of configuration
# -----------------------------------------------------------------------------

function(ck_finalize)
    _ck_state_get(project_name CK_PROJECT_NAME)
    _ck_state_get(generate_aggregate CK_PROJECT_GENERATE_AGGREGATE)
    _ck_state_get(games CK_GAMES)
    _ck_state_get(plugins CK_PLUGINS)
    _ck_state_get(modules CK_MODULES)
    _ck_state_get(owners CK_RESOURCE_OWNERS)

    if(NOT modules AND NOT games)
        message(WARNING "ck_finalize: no game or module declared in '${project_name}'.")
    endif()

    # Ownership is checked here rather than in ck_add_resource, so that resources
    # can be declared before the game or the module they belong to.
    foreach(owner IN LISTS owners)
        _ck_resource_owner_key(owner_key "${owner}")
        _ck_state_get(kind CK_RESOURCE_OWNER_${owner_key}_KIND)

        if(kind STREQUAL "GAME")
            if(NOT owner IN_LIST games)
                message(FATAL_ERROR
                    "ck_finalize: resources are declared for '${owner}', which is neither a "
                    "game nor a module declared before them.\n"
                    "Declared games: ${games}\n"
                    "Declared modules: ${modules}")
            endif()
        else()
            _ck_module_declared(declared "${owner}")
            if(NOT declared)
                message(FATAL_ERROR
                    "ck_finalize: resources are declared for '${owner}', which is not a "
                    "declared module. Declared modules: ${modules}")
            endif()
        endif()

        _ck_cook_resources("${owner}")
    endforeach()

    # What every owner declared as embedded, gathered into the one resource library
    # of the project. Both halves are emitted here, and in this order: the shaders
    # are what the library compiles in, and a generated file has to be produced by a
    # command sitting in the same directory scope as the one reading it.
    _ck_compile_shaders()
    _ck_embed_builtin_files()

    # Configuration and manifest happen here, because knowing which properties
    # apply to a game and which module resources it ships both require seeing the
    # whole link graph.
    foreach(game IN LISTS games)
        _ck_write_config(${game})
        _ck_write_manifest(${game})
    endforeach()

    # Last, and deliberately: what an install ships is decided from the interfaces
    # the whole configuration ended up with, so nothing may be linked after it.
    _ck_install_project()

    _ck_state_get(thirdparties CK_THIRDPARTIES)
    _ck_state_get(tests CK_TESTS)

    list(LENGTH modules module_count)
    message(STATUS "Cocktail: project '${project_name}'")

    # Said out loud, because a sanitizer build is not one to be in by accident: it is
    # several times slower and it holds every freed block back for a while, so a run
    # that behaves nothing like the last one has a reason here.
    _ck_sanitizer_enabled(sanitizer)
    if(sanitizer)
        message(STATUS "Cocktail: address sanitizer enabled (ENABLE_ASAN)")
        if(MSVC)
            # MSVC links the sanitizer against its own runtime DLL, which ships with the
            # toolchain and is not copied next to the games. A build run from a developer
            # prompt finds it, a game started from Explorer does not.
            message(STATUS
                "Cocktail: the MSVC sanitizer runtime is a DLL shipped with the toolchain, "
                "so a program built this way has to find it at startup. Run from a "
                "developer prompt, or put the MSVC bin directory on PATH.")
        endif()
    endif()

    message(STATUS "Cocktail: ${module_count} module(s) [${modules}]")
    if(generate_aggregate)
        _ck_state_get(aggregate_target CK_PROJECT_AGGREGATE_TARGET)
        message(STATUS "Cocktail: project aggregate target: ${aggregate_target}")
    endif()

    if(games)
        list(LENGTH games games_count)
        message(STATUS "Cocktail: ${games_count} games [${games}]")
    endif()

    if(plugins)
        list(LENGTH plugins plugins_count)
        message(STATUS "Cocktail: ${plugins_count} plugins [${plugins}]")
    endif()

    if(thirdparties)
        list(LENGTH thirdparties thirdparties_count)
        message(STATUS "Cocktail: ${thirdparties_count} third-party [${thirdparties}]")
    endif()

    if(tests)
        list(LENGTH tests tests_count)
        message(STATUS "Cocktail: ${tests_count} tests [${tests}]")
    endif()

    _ck_state_get(install_namespaces CK_INSTALL_NAMESPACES)
    if(install_namespaces)
        _ck_state_get(cmake_dir CK_INSTALL_CMAKE_DIR)
        message(STATUS
            "Cocktail: installs as ${cmake_dir}/${project_name}Config.cmake, exporting "
            "[${install_namespaces}]")
    endif()

    _ck_state_get(builtin_owners CK_BUILTIN_OWNERS)
    if(builtin_owners)
        list(LENGTH builtin_owners builtin_count)
        list(REMOVE_DUPLICATES builtin_owners)
        _ck_builtin_library(builtin_library)
        message(STATUS
            "Cocktail: ${builtin_count} embedded file(s) in ${builtin_library}, from "
            "[${builtin_owners}]")
    endif()
endfunction()
