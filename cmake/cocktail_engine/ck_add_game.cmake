# -----------------------------------------------------------------------------
# ck_add_game - executable built from sources/<NAME>
#
# A game links CocktailEngine::All, the engine it is built on, and <Project>::All
# when the project aggregates modules of its own, so it needs no DEPENDS to reach
# the runtime. DEPENDS is for what is not part of it: a plugin module, a
# third-party library.
#
# BASE_DIRECTORY groups games under a subdirectory of sources/, as in
# sources/games/Sandbox for BASE_DIRECTORY games, exactly as it does for a module.
# It is layout only: the target name, the binary and every include are left
# untouched, since the include root moves with the game.
# -----------------------------------------------------------------------------

function(ck_add_game name)
    cmake_parse_arguments(ARG "" "BASE_DIRECTORY" "DEPENDS" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "ck_add_game(${name}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    _ck_state_get(sources_dir CK_SOURCES_DIR)
    _ck_state_get(output_dir CK_OUTPUT_DIR)

    if(NOT name)
        message(FATAL_ERROR "ck_add_game: NAME is required.")
    endif()
    if(NOT sources_dir)
        message(FATAL_ERROR "ck_add_game: call ck_project() first.")
    endif()

    # A base directory is a place inside sources/, not a way out of it: an absolute
    # path or a '..' would put the game somewhere the framework does not own.
    if(ARG_BASE_DIRECTORY)
        if(IS_ABSOLUTE "${ARG_BASE_DIRECTORY}")
            message(FATAL_ERROR
                "ck_add_game(${name}): BASE_DIRECTORY '${ARG_BASE_DIRECTORY}' must be "
                "relative to the sources directory.")
        endif()
        if(ARG_BASE_DIRECTORY MATCHES "(^|/)\\.\\.(/|$)")
            message(FATAL_ERROR
                "ck_add_game(${name}): BASE_DIRECTORY '${ARG_BASE_DIRECTORY}' cannot "
                "leave the sources directory.")
        endif()
        # Written as 'games', 'games/' or 'games/samples' alike.
        string(REGEX REPLACE "^/+|/+$" "" ARG_BASE_DIRECTORY "${ARG_BASE_DIRECTORY}")
    endif()

    # Root the game hangs from: sources/, or the subdirectory of it that
    # BASE_DIRECTORY asks for.
    if(ARG_BASE_DIRECTORY)
        set(game_root "${sources_dir}/${ARG_BASE_DIRECTORY}")
    else()
        set(game_root "${sources_dir}")
    endif()

    set(source_dir "${game_root}/${name}")
    if(NOT IS_DIRECTORY "${source_dir}")
        message(FATAL_ERROR "ck_add_game(${name}): missing source directory: ${source_dir}")
    endif()

    _ck_collect_sources(sources "${source_dir}")
    if(NOT sources)
        message(FATAL_ERROR "ck_add_game(${name}): no source found in ${source_dir}")
    endif()

    add_executable(${name} ${sources})
    # The root the game sits in, not sources/ itself, so a file of the game is still
    # reached as <Game/Header.hpp> and BASE_DIRECTORY moves it without touching a
    # line of code.
    target_include_directories(${name} PRIVATE "${game_root}")

    # Shared modules are written to the same directory, so the game resolves them
    # from its own location and runs straight from the output directory.
    _ck_loader_path(loader_path)
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${output_dir}"
        BUILD_RPATH "${loader_path}"
        INSTALL_RPATH "${loader_path}"
        FOLDER "Games")

    # What a game is built on, and the one dependency it does not get to choose: the
    # main() it starts from, the allocator under it and the services it boots all come
    # from there. Its absence is therefore a project that cannot hold a game and not a
    # link to skip, hence the check: target_link_libraries would report it too, at
    # generate time and as a target that could not be found, which says nothing about
    # which of the two ways of reaching the engine went missing.
    if(NOT TARGET CocktailEngine::All)
        message(FATAL_ERROR
            "ck_add_game(${name}): CocktailEngine::All is not available, and it is what a "
            "game is built on.\n"
            "In a project consuming an installed engine, it is what find_package("
            "CocktailEngine) brings in.\n"
            "In the engine itself, it is the project aggregate, which ck_project() only "
            "declares when asked: add AGGREGATE_TARGET to it.")
    endif()
    target_link_libraries(${name} PRIVATE CocktailEngine::All)

    # The modules this project builds on top of the engine, when it aggregates them.
    # Skipped when the project is the engine: its own aggregate is the target just
    # linked, and naming it twice would only repeat it on the link line.
    _ck_state_get(generate_project_aggregate CK_PROJECT_GENERATE_AGGREGATE)
    if(generate_project_aggregate)
        _ck_project_aggregate(aggregate_target aggregate_alias)
        if(NOT aggregate_alias STREQUAL "CocktailEngine::All")
            target_link_libraries(${name} PRIVATE ${aggregate_alias})
        endif()
    endif()

    # Link explicit dependencies
    if(ARG_DEPENDS)
        target_link_libraries(${name} PRIVATE ${ARG_DEPENDS})
    endif()

    _ck_configure_target(${name})
    _ck_configure_sanitizer(${name})
    _ck_stage_external_modules(${name})

    _ck_state_append(CK_GAMES "${name}")
    _ck_resource_owner_key(key "${name}")
    _ck_state_set(CK_GAME_${key}_DIRECTORY "${source_dir}")
endfunction()
