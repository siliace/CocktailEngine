# -----------------------------------------------------------------------------
# ck_set_property - sets property values for a game
#
# Takes <Module>::<Property> <value> pairs, validated against the declared
# schema: a typo fails at configure time with the list of known names.
#
# Resolution happens immediately rather than being deferred, so the error points
# at the offending call. This requires declarations to come first, which is
# already the case: the engine declares on include, plugins on ck_add_plugin.
# -----------------------------------------------------------------------------

function(ck_set_property)
    cmake_parse_arguments(ARG "" "TARGET" "" ${ARGN})

    if(NOT ARG_TARGET)
        message(FATAL_ERROR "ck_set_property: TARGET is required.")
    endif()

    _ck_state_get(games CK_GAMES)

    if(NOT ARG_TARGET IN_LIST games)
        message(FATAL_ERROR
            "ck_set_property: '${ARG_TARGET}' is not a game. Property values belong to a "
            "game configuration, not to a module.")
    endif()

    set(pairs "${ARG_UNPARSED_ARGUMENTS}")
    list(LENGTH pairs pair_count)
    if(pair_count EQUAL 0)
        message(FATAL_ERROR "ck_set_property(${ARG_TARGET}): no property given.")
    endif()

    math(EXPR remainder "${pair_count} % 2")
    if(NOT remainder EQUAL 0)
        message(FATAL_ERROR
            "ck_set_property(${ARG_TARGET}): expected <property> <value> pairs, "
            "got ${pair_count} argument(s): ${pairs}")
    endif()

    while(pairs)
        list(POP_FRONT pairs reference value)
        _ck_resolve_property("${reference}" module property)
        _ck_validate_property_value("${module}" "${property}" "${value}")

        _ck_property_key(key "${module}" "${property}")
        _ck_state_set(CK_VALUE_${ARG_TARGET}_${key} "${value}")
        _ck_state_append(CK_VALUES_${ARG_TARGET} "${key}")
    endwhile()
endfunction()
