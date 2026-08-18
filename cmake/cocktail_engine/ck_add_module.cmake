# -----------------------------------------------------------------------------
# ck_add_module - declares a module
#
# Creates the real target <Namespace>_<Module> plus the <Namespace>::<Module>
# alias. The namespaced alias is what makes ck_add_game report a missing
# dependency right away instead of failing at generation time.
#
# A module is a class, <Module>Module, deriving from Module through a generated
# base that answers what the build knows about it. The class is written for the
# module when it does not exist yet, so a new module starts from its entry point,
# Startup and Shutdown included, rather than from an empty directory.
#
# A module is a shared library, because that is what the engine loads: a plugin
# dropped next to the game must be replaceable without relinking it. STATIC opts
# out, for a module that has no reason to be swapped at runtime and would rather
# be folded into its consumers.
#
# NAMESPACE overrides the one the scope provides. A plugin may not use it to
# borrow someone else's namespace, for the same reason it cannot declare their
# resources: a module belongs to whoever builds it.
#
# A module directory holds include/<Namespace>/<Module> for the headers it publishes,
# src/<Namespace>/<Module> for everything else, the entry point included, resources/
# for the assets it ships, thirdparty/ for the libraries it vendors, one subdirectory
# each, and tests/ for what ck_add_test compiles into a binary of its own. Only the
# two source halves are globbed, so what sits in the other three is never taken for
# module code. A .natvis sitting at the root of the module, or anywhere in its
# sources, is picked up as well and travels to every binary linking the module, so
# how the types of a module are displayed while debugging is one file to drop in.
# A CMakeLists.txt at the root of the module is
# picked up when it is there: that is where the module declares what it exposes to the
# rest of the project. Both source halves are include roots, so a file
# is reached as <CocktailEngine/Core/CoreModule.hpp>, mirroring the qualified name of
# the module, whatever the module directory is named or nested under.
#
# BASE_DIRECTORY groups modules under a subdirectory of sources/, as in
# sources/runtime/Core for BASE_DIRECTORY runtime. It is layout only: the qualified
# name, the binary, the resource subdirectory and every include are left untouched.
# Moving a module is therefore this one keyword, with nothing to update around it.
#
# MODULE_CLASS says whether the module has a module class at all, and is ON unless
# told otherwise: a module the engine loads is driven through that class. OFF is for
# code that is built as a module but is never loaded as one, a support library the
# rest of the project links being the case for it. Nothing is then scaffolded and no
# base is generated, so such a module brings its own sources.
#
# EXTERNAL declares a module that is already built and only needs to be known:
# no sources are read and no target is created. This is how an installed engine
# registers its modules, so that their properties resolve and their resources
# are found without this project rebuilding them.
# -----------------------------------------------------------------------------

function(ck_add_module name)
    cmake_parse_arguments(ARG "EXTERNAL;STATIC"
        "NAMESPACE;BASE_DIRECTORY;MODULE_CLASS" "DEPENDS" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "ck_add_module(${name}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    # Spelled out rather than left to if(), which reads an unknown word as a
    # variable name and would quietly take a misspelling for OFF.
    if(DEFINED ARG_MODULE_CLASS)
        set(module_class_given TRUE)
        if(NOT ARG_MODULE_CLASS MATCHES "^(ON|OFF|TRUE|FALSE|YES|NO|0|1)$")
            message(FATAL_ERROR
                "ck_add_module(${name}): MODULE_CLASS expects a boolean, got "
                "'${ARG_MODULE_CLASS}'.")
        endif()
    else()
        set(module_class_given FALSE)
        set(ARG_MODULE_CLASS ON)
    endif()

    if(name MATCHES "::")
        message(FATAL_ERROR
            "ck_add_module(${name}): pass the bare module name. The namespace comes from "
            "the enclosing plugin or project, or from NAMESPACE.")
    endif()

    # A base directory is a place inside sources/, not a way out of it: an absolute
    # path or a '..' would put the module somewhere the framework does not own.
    if(ARG_BASE_DIRECTORY)
        if(IS_ABSOLUTE "${ARG_BASE_DIRECTORY}")
            message(FATAL_ERROR
                "ck_add_module(${name}): BASE_DIRECTORY '${ARG_BASE_DIRECTORY}' must be "
                "relative to the sources directory.")
        endif()
        if(ARG_BASE_DIRECTORY MATCHES "(^|/)\\.\\.(/|$)")
            message(FATAL_ERROR
                "ck_add_module(${name}): BASE_DIRECTORY '${ARG_BASE_DIRECTORY}' cannot "
                "leave the sources directory.")
        endif()
        # Written as 'runtime', 'runtime/' or 'runtime/gameplay' alike.
        string(REGEX REPLACE "^/+|/+$" "" ARG_BASE_DIRECTORY "${ARG_BASE_DIRECTORY}")
    endif()

    if(ARG_NAMESPACE)
        if(CK_CURRENT_PLUGIN AND NOT ARG_NAMESPACE STREQUAL CK_CURRENT_PLUGIN)
            message(FATAL_ERROR
                "ck_add_module(${name}): plugin '${CK_CURRENT_PLUGIN}' cannot declare a "
                "module in namespace '${ARG_NAMESPACE}'. A module belongs to whoever "
                "builds it.")
        endif()
        set(namespace "${ARG_NAMESPACE}")
    else()
        _ck_current_namespace(namespace)
        if(NOT namespace)
            message(FATAL_ERROR
                "ck_add_module(${name}): call ck_project() first, or pass NAMESPACE.")
        endif()
    endif()

    set(module "${namespace}::${name}")
    _ck_module_declared(declared "${module}")
    if(declared)
        message(FATAL_ERROR "ck_add_module: '${module}' is already declared.")
    endif()

    _ck_current_sources_dir(sources_dir)
    set(target "${namespace}-${name}")

    if(ARG_EXTERNAL)
        if(ARG_STATIC OR ARG_DEPENDS OR ARG_BASE_DIRECTORY OR module_class_given)
            message(FATAL_ERROR
                "ck_add_module(${name}): an EXTERNAL module is already built, STATIC, "
                "DEPENDS, BASE_DIRECTORY and MODULE_CLASS do not apply to it.")
        endif()
        # An already built module usually comes with an imported target of the very
        # qualified name it is declared under, which is what find_package brings in.
        # Picking it up here is what puts an installed module back in the link graph:
        # reachability, and the staging of its binary next to the programs that load
        # it, then follow the same rule they follow for a module built here.
        if(TARGET ${module})
            set(target "${module}")
        else()
            set(target "")
        endif()
        set(linkage "")
        # Nothing of ours holds its class, whether it has one or not.
        set(ARG_MODULE_CLASS "")
    else()
        if(NOT sources_dir)
            message(FATAL_ERROR "ck_add_module(${name}): call ck_project() first.")
        endif()
        if(TARGET ${target})
            message(FATAL_ERROR
                "ck_add_module(${name}): target '${target}' already exists.")
        endif()
        if(ARG_STATIC)
            set(linkage STATIC)
        else()
            set(linkage SHARED)
        endif()

        # Root the module hangs from: sources/, or the subdirectory of it that
        # BASE_DIRECTORY asks for.
        if(ARG_BASE_DIRECTORY)
            set(module_root "${sources_dir}/${ARG_BASE_DIRECTORY}")
        else()
            set(module_root "${sources_dir}")
        endif()

        # Both halves of the layout are created, whether the module is new or only
        # uses one of them: they are include paths, and an include path that does
        # not exist is a warning waiting to happen. resources/ and thirdparty/ come
        # along so that where the assets of a module go, and where the libraries it
        # vendors go, are part of the module from the start rather than something to
        # remember about later.
        set(source_dir "${module_root}/${name}")
        file(MAKE_DIRECTORY
            "${source_dir}/include/${namespace}/${name}"
            "${source_dir}/src/${namespace}/${name}"
            "${source_dir}/resources"
            "${source_dir}/tests"
            "${source_dir}/thirdparty")

        _ck_declare_file("${source_dir}/CMakeLists.txt")

        _ck_keep_directory("${source_dir}/resources")
        _ck_keep_directory("${source_dir}/tests")
        _ck_keep_directory("${source_dir}/thirdparty")

        # A module that has nothing yet gets its class written for it, rather than
        # a build error about an empty directory. It happens before the sources are
        # collected: a CONFIGURE_DEPENDS glob refuses to see the directory it
        # already cached change during the same configure run.
        if(ARG_MODULE_CLASS)
            _ck_scaffold_module_class("${namespace}" "${name}" "${source_dir}")
        endif()

        _ck_collect_sources(sources "${source_dir}/include" "${source_dir}/src")
        if(NOT sources)
            # Only reachable for a module that asked for no class, since one that has
            # a class has just been given it.
            message(FATAL_ERROR
                "ck_add_module(${name}): no source found in ${source_dir}, neither under "
                "include/ nor under src/. A module declared MODULE_CLASS OFF gets nothing "
                "written for it, so it has to bring sources of its own.")
        endif()

        add_library(${target} ${linkage} ${sources})
        add_library(${module} ALIAS ${target})

        # What the module publishes, and only that: include/ is PUBLIC, so a
        # consumer sees <Namespace/Module/Header.hpp> and nothing else, and it
        # travels with the link, so seeing a module means depending on it. src/ is
        # PRIVATE and laid out the same way, so an internal header is included the
        # same as a published one and is reachable from nowhere else. Where the
        # module sits in sources/ never shows up in an include, so BASE_DIRECTORY
        # moves it without touching a line of code.
        # The published half is named twice, once for each side of the module's life:
        # the directory it is read from while this project builds it, and the one the
        # headers were copied to when the module was installed. A consumer of an
        # installed module therefore resolves the same includes without the source
        # tree it was built from being anywhere in sight.
        _ck_state_get(install_include_dir CK_INSTALL_INCLUDE_DIR)
        target_include_directories(${target}
            PUBLIC
                "$<BUILD_INTERFACE:${source_dir}/include>"
                "$<INSTALL_INTERFACE:${install_include_dir}>"
            PRIVATE "${source_dir}/src")
        if(CK_CURRENT_PLUGIN)
            set(folder "Plugins/${CK_CURRENT_PLUGIN}")
        else()
            set(folder "Modules")
        endif()
        # Binaries are named <namespace>-<module>, lowercased, so a module sitting
        # in the output directory says which project or plugin it comes from and two
        # modules of the same name never overwrite each other. The separator is a
        # hyphen rather than the '::' of the qualified name or a dot, since a dot
        # reads as the start of the extension on every platform.
        #
        # Lowercase because a file name is not a C++ name: it is compared by the
        # filesystem, and one that folds case would let two modules whose names
        # differ only in case land on the same file on Windows while staying
        # distinct on Linux. Naming them lowercased from the start makes that
        # collision the same on every platform instead of a Windows-only surprise.
        #
        # No 'lib' prefix either: it is what a Unix linker looks for behind -lname,
        # and a module is never linked that way. The engine loads it by file name,
        # so the file is named the one way everywhere rather than spelling the
        # platform's convention into whoever has to find it.
        string(TOLOWER "${namespace}-${name}" output_name)
        set_target_properties(${target} PROPERTIES
            OUTPUT_NAME "${output_name}"
            PREFIX ""
            IMPORT_PREFIX ""
            POSITION_INDEPENDENT_CODE ON
            # What the module is called once installed: the bare name, since the
            # namespace of the export set puts it back in front. A consumer therefore
            # links the same <Namespace>::<Module> whether the module was built beside
            # it or found already built.
            EXPORT_NAME "${name}"
            FOLDER "${folder}")

        if(linkage STREQUAL "SHARED")
            _ck_configure_shared_module(${target} "${namespace}" "${name}")
        endif()
        # The base holds what the build knows about the module, and it is a base of
        # the module class: with no class to derive it, there is nothing to generate.
        if(ARG_MODULE_CLASS)
            _ck_generate_module_base(${target} "${namespace}" "${name}")
        endif()
        # A binary of its own, hence its own allocation operators. A static module
        # takes those of whatever binary it ends up in.
        if(linkage STREQUAL "SHARED")
            _ck_generate_memory_overrides(${target} "${namespace}" "${name}")
        endif()
        # Whatever the module says about how its types are displayed, passed on to
        # every binary that links it.
        _ck_attach_visualisers(${target} "${namespace}" "${name}" "${source_dir}")

        if(ARG_DEPENDS)
            target_link_libraries(${target} PUBLIC ${ARG_DEPENDS})
        endif()
        _ck_configure_target(${target})
        _ck_configure_sanitizer(${target})
    endif()

    _ck_module_key(key "${module}")
    _ck_state_append(CK_MODULES "${module}")
    _ck_state_set(CK_MODULE_${key}_NAME "${name}")
    _ck_state_set(CK_MODULE_${key}_NAMESPACE "${namespace}")
    _ck_state_set(CK_MODULE_${key}_TARGET "${target}")
    # Empty for an external module: it is already built, and there is no directory
    # of ours holding it.
    _ck_state_set(CK_MODULE_${key}_DIRECTORY "${source_dir}")
    _ck_state_set(CK_MODULE_${key}_PLUGIN "${CK_CURRENT_PLUGIN}")
    _ck_state_set(CK_MODULE_${key}_LINKAGE "${linkage}")
    _ck_state_set(CK_MODULE_${key}_BASE_DIRECTORY "${ARG_BASE_DIRECTORY}")
    # Empty for an external module, like its target and its directory.
    _ck_state_set(CK_MODULE_${key}_MODULE_CLASS "${ARG_MODULE_CLASS}")

    # A module the project builds itself is part of what its games link. An EXTERNAL
    # one is reached through the aggregate of the project that built it, and a plugin
    # one is opt-in.
    if(target AND NOT ARG_EXTERNAL AND NOT CK_CURRENT_PLUGIN)
        _ck_join_project_aggregate("${module}")
    endif()

    # A module gets the last word on itself. Its CMakeLists.txt, when it has one, is
    # processed here: the target exists, the module is declared, so the file is free
    # to declare the properties the module exposes, vendor the libraries it needs, or
    # reach its own target directly for what the framework has no opinion about.
    # Having it optional is what keeps a module that has nothing to say silent.
    #
    # It is brought in with add_subdirectory rather than include(), so that a relative
    # path inside it resolves against the module, and so that its own build artifacts
    # land in a directory of their own.
    if(NOT ARG_EXTERNAL AND EXISTS "${source_dir}/CMakeLists.txt")
        # Read from the module scope, the way CK_CURRENT_PLUGIN is read from a plugin
        # one: the file states what it wants, never where it is.
        set(CK_CURRENT_MODULE "${module}")
        set(CK_CURRENT_MODULE_TARGET "${target}")
        set(CK_CURRENT_MODULE_DIR "${source_dir}")

        add_subdirectory("${source_dir}"
            "${CMAKE_CURRENT_BINARY_DIR}/modules/${namespace}/${name}")
    endif()
endfunction()
