# -----------------------------------------------------------------------------
# ck_declare_property - called by the module that owns the property
# -----------------------------------------------------------------------------

function(ck_declare_property)
    # SCOPE is still parsed, only to reject it: a property used to be able to ask
    # for compile-time resolution, and silently ignoring the keyword would leave a
    # module believing it still gets a define.
    cmake_parse_arguments(ARG "REQUIRED"
        "MODULE;NAME;TYPE;DEFAULT;SCOPE;DESCRIPTION" "VALUES" ${ARGN})

    if(NOT ARG_NAME)
        message(FATAL_ERROR "ck_declare_property: NAME is required.")
    endif()

    if(ARG_SCOPE)
        message(FATAL_ERROR
            "ck_declare_property(${ARG_NAME}): SCOPE is gone, every property is resolved at "
            "runtime from the generated <Game>.ckconfig.yml. Drop 'SCOPE ${ARG_SCOPE}'.")
    endif()

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "ck_declare_property(${ARG_NAME}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    if(NOT ARG_MODULE)
        message(FATAL_ERROR "ck_declare_property(${ARG_NAME}): MODULE is required.")
    endif()

    # MODULE may be the bare module name: the namespace of the current scope, the
    # enclosing plugin or the project, qualifies it unambiguously.
    if(NOT ARG_MODULE MATCHES "::")
        _ck_current_namespace(namespace)
        if(NOT namespace)
            message(FATAL_ERROR
                "ck_declare_property(${ARG_NAME}): '${ARG_MODULE}' has no namespace. Call "
                "ck_project() first, or use the <Module>::<Property> form.")
        endif()
        set(ARG_MODULE "${namespace}::${ARG_MODULE}")
    endif()

    if(NOT ARG_TYPE)
        set(ARG_TYPE STRING)
    endif()
    if(NOT ARG_TYPE MATCHES "^(ENUM|BOOL|INT|STRING)$")
        message(FATAL_ERROR
            "ck_declare_property(${ARG_NAME}): invalid TYPE '${ARG_TYPE}' "
            "(ENUM, BOOL, INT, STRING).")
    endif()
    if(ARG_TYPE STREQUAL "ENUM" AND NOT ARG_VALUES)
        message(FATAL_ERROR "ck_declare_property(${ARG_NAME}): TYPE ENUM requires VALUES.")
    endif()

    _ck_property_key(key "${ARG_MODULE}" "${ARG_NAME}")
    _ck_state_get(existing CK_PROPERTY_${key}_MODULE)
    if(existing)
        message(FATAL_ERROR
            "ck_declare_property: '${ARG_MODULE}::${ARG_NAME}' is already declared.")
    endif()

    _ck_state_set(CK_PROPERTY_${key}_MODULE "${ARG_MODULE}")
    _ck_state_set(CK_PROPERTY_${key}_NAME "${ARG_NAME}")
    _ck_state_set(CK_PROPERTY_${key}_TYPE "${ARG_TYPE}")
    _ck_state_set(CK_PROPERTY_${key}_VALUES "${ARG_VALUES}")
    _ck_state_set(CK_PROPERTY_${key}_DEFAULT "${ARG_DEFAULT}")
    _ck_state_set(CK_PROPERTY_${key}_DESCRIPTION "${ARG_DESCRIPTION}")
    _ck_state_set(CK_PROPERTY_${key}_REQUIRED "${ARG_REQUIRED}")

    if(ARG_DEFAULT)
        _ck_validate_property_value("${ARG_MODULE}" "${ARG_NAME}" "${ARG_DEFAULT}")
    endif()

    _ck_state_append(CK_PROPERTIES "${ARG_MODULE}::${ARG_NAME}")
    _ck_state_append(CK_PROPERTY_OWNERS_${ARG_NAME} "${ARG_MODULE}")
    _ck_state_append(CK_PROPERTY_KEYS_${ARG_MODULE} "${key}")
endfunction()
