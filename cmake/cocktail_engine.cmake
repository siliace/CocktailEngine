# =============================================================================
# Cocktail Engine - CMake integration layer
#
# Public facade for the framework commands. Components live in the directory
# named after this file stem, so the same facade works as cocktail_engine.cmake
# in the source tree and as <Project>Macros.cmake in an installed package.
# =============================================================================

include_guard(GLOBAL)

if(CMAKE_VERSION VERSION_LESS 3.21)
    message(FATAL_ERROR "Cocktail Engine requires CMake 3.21 or newer (cmake_language(DEFER)).")
endif()

# Remember the facade and its components while this file is being read. A function
# would see the list file of its caller instead of the framework file being loaded.
set_property(GLOBAL PROPERTY CK_FRAMEWORK_FILE "${CMAKE_CURRENT_LIST_FILE}")
get_filename_component(_ck_framework_stem "${CMAKE_CURRENT_LIST_FILE}" NAME_WE)
set(_ck_framework_component_dir
    "${CMAKE_CURRENT_LIST_DIR}/${_ck_framework_stem}")
set_property(GLOBAL PROPERTY CK_FRAMEWORK_COMPONENT_DIR
    "${_ck_framework_component_dir}")

set(_ck_framework_components
    private_helpers.cmake
    ck_project.cmake
    ck_add_plugin.cmake
    ck_add_module.cmake
    ck_declare_thirdparty.cmake
    ck_add_test.cmake
    ck_add_game.cmake
    ck_declare_property.cmake
    ck_set_property.cmake
    ck_get_property.cmake
    ck_add_resource.cmake
    ck_finalize.cmake
    ck_add_shader.cmake
)
set_property(GLOBAL PROPERTY CK_FRAMEWORK_COMPONENTS
    "${_ck_framework_components}")

include("${_ck_framework_component_dir}/private_helpers.cmake")
include("${_ck_framework_component_dir}/ck_project.cmake")
include("${_ck_framework_component_dir}/ck_add_plugin.cmake")
include("${_ck_framework_component_dir}/ck_add_module.cmake")
include("${_ck_framework_component_dir}/ck_declare_thirdparty.cmake")
include("${_ck_framework_component_dir}/ck_add_test.cmake")
include("${_ck_framework_component_dir}/ck_add_game.cmake")
include("${_ck_framework_component_dir}/ck_declare_property.cmake")
include("${_ck_framework_component_dir}/ck_set_property.cmake")
include("${_ck_framework_component_dir}/ck_get_property.cmake")
include("${_ck_framework_component_dir}/ck_add_resource.cmake")
include("${_ck_framework_component_dir}/ck_finalize.cmake")
include("${_ck_framework_component_dir}/ck_add_shader.cmake")

unset(_ck_framework_components)
unset(_ck_framework_component_dir)
unset(_ck_framework_stem)
