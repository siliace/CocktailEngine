# -----------------------------------------------------------------------------
# ck_get_property - effective value (explicitly set, otherwise the default)
# -----------------------------------------------------------------------------

function(ck_get_property out_var)
    cmake_parse_arguments(ARG "" "TARGET;MODULE;PROPERTY" "" ${ARGN})

    if(NOT ARG_TARGET OR NOT ARG_MODULE OR NOT ARG_PROPERTY)
        message(FATAL_ERROR "ck_get_property: TARGET, MODULE and PROPERTY are required.")
    endif()
    _ck_resolve_property("${ARG_MODULE}::${ARG_PROPERTY}" module property)

    _ck_property_key(key "${module}" "${property}")
    _ck_state_get(value CK_VALUE_${ARG_TARGET}_${key})
    if(value STREQUAL "")
        _ck_state_get(value CK_PROPERTY_${key}_DEFAULT)
    endif()
    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()
