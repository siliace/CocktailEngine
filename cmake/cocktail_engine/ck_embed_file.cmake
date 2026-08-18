# =============================================================================
# Embedded files
#
# ck_embed_file carries a file of the source tree inside every program the project
# builds, read back through the embedded filesystem instead of from the disk. It is
# what ck_add_shader does once the SPIR-V exists, offered for the files nothing has
# to be run on: the font a debug overlay draws with, the texture a renderer falls
# back to, the description a subsystem reads before a game has mounted anything.
#
# What goes in is what a module cannot run without. A module missing it does not
# work at all, so shipping it beside the binary would only give someone the chance
# to remove it, and the engine would have nothing to fall back on. Content a game
# provides or replaces is the other mechanism, ck_add_resource: an asset is cooked,
# written next to the executable and listed in the manifest of the game shipping it.
#
# The owner is whoever the call was made from, a module or a game, and TARGET is
# only needed where no scope says which: at project level, or when one owner
# declares a file on behalf of another it is allowed to.
#
# The file is read from the resources/ directory of its owner and published under
# the path it is written at, so the source tree says what the code asks for:
#
#   ck_embed_file(FILE fonts/Roboto-Regular.ttf)
#
#   -> builtin://<module>/resources/fonts/Roboto-Regular.ttf
#
# OUTPUT publishes it somewhere else, for when the path the code wants is not the
# path the file happens to be filed under:
#
#   ck_embed_file(FILE fonts/Roboto-Regular.ttf OUTPUT fonts/default.ttf)
#
#   -> builtin://<module>/resources/fonts/default.ttf
#
# One call is one file, which is the same deliberate limit ck_add_resource has: a
# directory form would embed whatever happened to be dropped in it, and what every
# program of the project carries is not something to grow by accident.
# =============================================================================

function(ck_embed_file)
    cmake_parse_arguments(ARG "" "TARGET;FILE;OUTPUT" "" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ck_embed_file: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT ARG_FILE)
        message(FATAL_ERROR "ck_embed_file: FILE is required.")
    endif()

    # A file of the owner, named the way the owner files it: the resources/ directory
    # is the root of what it embeds, and nothing above it is the owner's to publish.
    if(IS_ABSOLUTE "${ARG_FILE}" OR ARG_FILE MATCHES "(^|/)\\.\\.(/|$)")
        message(FATAL_ERROR
            "ck_embed_file: FILE '${ARG_FILE}' is read from the resources of its owner. "
            "It cannot be absolute nor leave that directory.")
    endif()

    _ck_embedded_owner("ck_embed_file(${ARG_FILE})" "${ARG_TARGET}" owner resources_dir)

    set(source_file "${resources_dir}/${ARG_FILE}")
    if(NOT EXISTS "${source_file}")
        message(FATAL_ERROR
            "ck_embed_file(${ARG_FILE}): file not found: ${source_file}\n"
            "Embedded files of ${owner} are read from ${resources_dir}")
    endif()
    # A directory would be embedded as the one file it is not, and cmrc would fail on
    # it halfway through generating the build. Said here, where the call is.
    if(IS_DIRECTORY "${source_file}")
        message(FATAL_ERROR
            "ck_embed_file(${ARG_FILE}): '${source_file}' is a directory. One call "
            "embeds one file.")
    endif()

    # Published where it is written unless told otherwise, which is what makes the
    # layout of resources/ the one the code reads.
    if(NOT ARG_OUTPUT)
        set(ARG_OUTPUT "${ARG_FILE}")
    endif()
    if(IS_ABSOLUTE "${ARG_OUTPUT}" OR ARG_OUTPUT MATCHES "(^|/)\\.\\.(/|$)")
        message(FATAL_ERROR
            "ck_embed_file(${ARG_FILE}): OUTPUT '${ARG_OUTPUT}' is the path the engine is "
            "asked for, rooted at the resources of ${owner}. It cannot be absolute nor "
            "leave that root.")
    endif()

    _ck_claim_embedded_path("${owner}" "${ARG_OUTPUT}" "ck_embed_file")

    # Everything above says whether the call makes sense, and says so from the call
    # itself so the error names the module that wrote it. The copy into the embedded
    # directory is only recorded: it is emitted by ck_finalize, next to the resource
    # library reading it, since a build system is only handed a generated file and
    # the command making it when both were declared in the same directory.
    _ck_embedded_file(staged_file "${owner}" "${ARG_OUTPUT}")

    string(MAKE_C_IDENTIFIER "${owner}_${ARG_OUTPUT}" key)
    _ck_state_append(CK_EMBED_KEYS "${key}")
    _ck_state_set(CK_EMBED_${key}_OWNER "${owner}")
    _ck_state_set(CK_EMBED_${key}_PATH "${ARG_OUTPUT}")
    _ck_state_set(CK_EMBED_${key}_SOURCE "${source_file}")
    _ck_state_set(CK_EMBED_${key}_FILE "${staged_file}")

    _ck_record_embedded_file("${owner}" "${staged_file}")
endfunction()
