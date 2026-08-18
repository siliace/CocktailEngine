# -----------------------------------------------------------------------------
# ck_add_test - test binary of a module
#
# Called from the CMakeLists.txt of a module, like ck_declare_property and
# ck_declare_thirdparty, and for the same reason: the scope already says which
# module is meant, so the call has nothing to name.
#
# Tests live in tests/ at the root of the module, which is the one half of the
# layout the module glob does not read. What sits there is therefore compiled into
# a binary of its own and never into the module, and platform directories work in
# it exactly as they do in src/: a test that only holds on one platform sits in a
# directory saying which.
#
# The binary links the module and Catch2, which ck_project brings in, and it sees
# the private half of the module as well: tests are not a consumer of the module,
# they are the one thing allowed to reach what it does not publish.
#
# It is a program of this framework like a game is, so it takes its main() from the
# same place and runs its cases from inside a booted application rather than from a
# main of the test framework's own. What a test sees is therefore what a game sees:
# the services registered, the allocator in place, exceptions caught and logged.
#
# It is written next to the modules, since those are shared libraries it has to find
# at runtime, and registered with CTest as a single entry. The framework does not run
# it at build time to look inside: what cases it holds is the test framework's
# business, and a binary that has to be run before it can be listed turns a broken
# test into a broken build.
# -----------------------------------------------------------------------------

function(ck_add_test)
    cmake_parse_arguments(ARG "" "" "DEPENDS" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "ck_add_test: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT CK_CURRENT_MODULE)
        message(FATAL_ERROR
            "ck_add_test: call it from the CMakeLists.txt of a module, which is what tells "
            "the tests what they are testing.")
    endif()

    # Not building the tests is a normal state, not something to report: a project
    # consuming the engine configures its modules and leaves their tests alone.
    if(NOT CK_BUILD_TESTS)
        return()
    endif()

    _ck_module_key(key "${CK_CURRENT_MODULE}")
    _ck_state_get(namespace CK_MODULE_${key}_NAMESPACE)
    _ck_state_get(name CK_MODULE_${key}_NAME)
    _ck_state_get(output_dir CK_OUTPUT_DIR)

    set(tests_dir "${CK_CURRENT_MODULE_DIR}/tests")
    _ck_collect_sources(sources "${tests_dir}")
    if(NOT sources)
        message(FATAL_ERROR "ck_add_test: no source found in ${tests_dir}")
    endif()

    set(target "${namespace}-${name}-Tests")
    if(TARGET ${target})
        message(FATAL_ERROR
            "ck_add_test: ${CK_CURRENT_MODULE} already has a test binary.")
    endif()

    _ck_generate_test_main(test_main "${namespace}" "${name}")

    add_executable(${target} ${sources} "${test_main}")

    # Named like the module it tests, and lowercased for the same reason: it lands in
    # the same directory, next to the shared modules it loads from its own location.
    string(TOLOWER "${namespace}-${name}-tests" output_name)
    _ck_loader_path(loader_path)
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${output_name}"
        RUNTIME_OUTPUT_DIRECTORY "${output_dir}"
        BUILD_RPATH "${loader_path}"
        INSTALL_RPATH "${loader_path}"
        FOLDER "Tests")

    # The entry point module comes before Catch2, and both after the module under test:
    # the main() it holds is what the C runtime asks for, and what that main calls is
    # compiled into the binary itself, so nothing has to be found back in an archive
    # already walked past.
    _ck_entry_point_provider(entry_point)
    target_link_libraries(${target} PRIVATE
        ${CK_CURRENT_MODULE} ${entry_point} Catch2::Catch2 ${ARG_DEPENDS})
    target_include_directories(${target} PRIVATE "${CK_CURRENT_MODULE_DIR}/src")
    _ck_configure_target(${target})
    _ck_configure_sanitizer(${target})
    _ck_stage_external_modules(${target})

    # Run from the output directory, which is where a module looks for the resources
    # it ships, so a test sees the same layout the game will.
    add_test(NAME ${CK_CURRENT_MODULE} COMMAND ${target} WORKING_DIRECTORY "${output_dir}")

    _ck_state_append(CK_TESTS "${CK_CURRENT_MODULE}")
endfunction()
