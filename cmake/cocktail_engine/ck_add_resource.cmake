# -----------------------------------------------------------------------------
# ck_add_resource - declares an asset to cook
#
# Only registration happens here. Cooking commands are emitted by ck_finalize,
# which makes the order relative to ck_add_game or ck_add_module irrelevant and
# allows logical name collisions to be detected globally.
#
# TODO this one-call-per-file form will not scale. Add ck_add_resource_directory()
# and keep this one only for assets that need specific import options.
# -----------------------------------------------------------------------------

function(ck_add_resource)
    cmake_parse_arguments(ARG "" "TARGET;NAME;FILE" "OPTIONS" ${ARGN})

    if(NOT ARG_TARGET)
        message(FATAL_ERROR "ck_add_resource: TARGET is required.")
    endif()
    if(NOT ARG_FILE)
        message(FATAL_ERROR "ck_add_resource: FILE is required.")
    endif()
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ck_add_resource: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    _ck_resolve_resource_owner("${ARG_TARGET}" owner kind resources_dir)

    # Logical name derived from the path by default:
    #   Models/Sponza.gltf -> Models/Sponza
    if(NOT ARG_NAME)
        string(REGEX REPLACE "\\.[^./]*$" "" ARG_NAME "${ARG_FILE}")
    endif()

    # The qualified form is built by the framework, never by the user: allowing
    # '::' in a name would let a game forge a plugin-looking name, which is
    # exactly the collision the namespace is there to prevent.
    if(ARG_NAME MATCHES "::")
        message(FATAL_ERROR
            "ck_add_resource: '${ARG_NAME}' cannot contain '::'. Qualification is derived "
            "from the owning module.")
    endif()

    set(source_file "${resources_dir}/${ARG_FILE}")
    if(NOT EXISTS "${source_file}")
        message(FATAL_ERROR
            "ck_add_resource(${ARG_NAME}): file not found: ${source_file}\n"
            "Resources of ${owner} are read from ${resources_dir}")
    endif()

    _ck_resource_key(key "${owner}" "${ARG_NAME}")
    _ck_state_get(known CK_RESOURCE_${key}_FILE)
    if(known)
        message(FATAL_ERROR
            "ck_add_resource: logical name '${ARG_NAME}' is already declared for ${owner}.")
    endif()

    _ck_resource_owner_key(owner_key "${owner}")
    _ck_state_get(registered CK_RESOURCE_OWNER_${owner_key}_KIND)
    if(NOT registered)
        # A game and a module never share an output directory, even if the game
        # happens to be named like a module.
        if(kind STREQUAL "MODULE")
            string(REPLACE "::" "/" subdir "modules/${owner}")
            set(namespace "${owner}::")
        else()
            set(subdir "games/${owner}")
            set(namespace "")
        endif()

        _ck_state_append(CK_RESOURCE_OWNERS "${owner}")
        _ck_state_set(CK_RESOURCE_OWNER_${owner_key}_KIND "${kind}")
        _ck_state_set(CK_RESOURCE_OWNER_${owner_key}_RESOURCES_DIR "${resources_dir}")
        _ck_state_set(CK_RESOURCE_OWNER_${owner_key}_SUBDIR "${subdir}")
        _ck_state_set(CK_RESOURCE_OWNER_${owner_key}_NAMESPACE "${namespace}")
    endif()

    _ck_state_append(CK_RESOURCES_${owner_key} "${ARG_NAME}")
    _ck_state_set(CK_RESOURCE_${key}_FILE "${ARG_FILE}")
    _ck_state_set(CK_RESOURCE_${key}_OPTIONS "${ARG_OPTIONS}")
endfunction()
