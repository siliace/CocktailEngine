# =============================================================================
# Shaders
#
# A shader is source code the module owns, not an asset: it is written against the
# renderer of the module and compiled with it, so it is read from the resources/
# directory of that module and embedded in its binary. What ships is SPIR-V, and
# the GLSL never leaves the source tree.
#
# One call is one compilation, and not one file: a shader written once is compiled
# as many times as it has variants, each with its own macros and its own name.
# That is why OUTPUT is a name of its own rather than something derived from the
# input, and why it is the path the engine is asked for:
#
#   ck_add_shader(FILE shaders/mesh/basic.vert
#                 OUTPUT shaders/mesh/basic/textured.vert.spv
#                 MACRO CK_TEXTURED=1)
#
#   -> builtin://<module>/resources/shaders/mesh/basic/textured.vert.spv
# =============================================================================

function(ck_add_shader)
    cmake_parse_arguments(ARG "" "TARGET;FILE;TYPE;OUTPUT" "MACRO" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ck_add_shader: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT ARG_FILE)
        message(FATAL_ERROR "ck_add_shader: FILE is required.")
    endif()

    # A shader belongs to whoever wrote the renderer it feeds, which is the module
    # the call was made from unless it says otherwise.
    _ck_embedded_owner("ck_add_shader(${ARG_FILE})" "${ARG_TARGET}" owner resources_dir)

    set(source_file "${resources_dir}/${ARG_FILE}")
    if(NOT EXISTS "${source_file}")
        message(FATAL_ERROR
            "ck_add_shader(${ARG_FILE}): file not found: ${source_file}\n"
            "Shaders of ${owner} are read from ${resources_dir}")
    endif()

    if(NOT ARG_TYPE)
        _ck_shader_stage(ARG_TYPE "${ARG_FILE}")
        if(NOT ARG_TYPE)
            message(FATAL_ERROR
                "ck_add_shader(${ARG_FILE}): the extension does not name a pipeline "
                "stage, so TYPE has to say which one it is.")
        endif()
    endif()

    # Defaults to the input with the SPIR-V extension appended, which is what a
    # shader having a single variant wants.
    if(NOT ARG_OUTPUT)
        set(ARG_OUTPUT "${ARG_FILE}.spv")
    endif()
    if(IS_ABSOLUTE "${ARG_OUTPUT}" OR ARG_OUTPUT MATCHES "(^|/)\\.\\.(/|$)")
        message(FATAL_ERROR
            "ck_add_shader(${ARG_FILE}): OUTPUT '${ARG_OUTPUT}' is the path the engine is "
            "asked for, rooted at the resources of ${owner}. It cannot be absolute nor "
            "leave that root.")
    endif()

    if(NOT TARGET glslang-standalone)
        message(FATAL_ERROR
            "ck_add_shader(${ARG_FILE}): the GLSL compiler is not available. glslang is "
            "vendored at project level, and its 'glslang-standalone' target is what "
            "compiles a shader.")
    endif()

    # Each variant of a shader needs an OUTPUT of its own, since that is the path it
    # is asked for. Claimed rather than merely compared, so that a variant and an
    # embedded file picking the same path are the one error.
    _ck_claim_embedded_path("${owner}" "${ARG_OUTPUT}" "ck_add_shader")

    # Compiled straight into the embedded directory, at the very path the engine is
    # asked for: the SPIR-V is what ships, so where it is produced is where it is
    # published from.
    _ck_embedded_file(output_file "${owner}" "${ARG_OUTPUT}")

    # Everything above says whether the call makes sense, and says so from the call
    # itself so the error names the module that wrote it. The compilation is only
    # recorded: it is emitted by ck_finalize, next to the resource library reading
    # its output, since a build system is only handed a generated file and the
    # command making it when both were declared in the same directory.
    _ck_shader_key(key "${owner}" "${ARG_OUTPUT}")
    _ck_state_append(CK_SHADER_KEYS "${key}")
    _ck_state_set(CK_SHADER_${key}_OWNER "${owner}")
    _ck_state_set(CK_SHADER_${key}_NAME "${ARG_OUTPUT}")
    _ck_state_set(CK_SHADER_${key}_SOURCE "${source_file}")
    _ck_state_set(CK_SHADER_${key}_BINARY "${output_file}")
    _ck_state_set(CK_SHADER_${key}_STAGE "${ARG_TYPE}")
    _ck_state_set(CK_SHADER_${key}_MACRO "${ARG_MACRO}")

    # The SPIR-V is embedded rather than installed: what depends on it is the
    # resource library, so building a program compiles the shader.
    _ck_record_embedded_file("${owner}" "${output_file}")
endfunction()
