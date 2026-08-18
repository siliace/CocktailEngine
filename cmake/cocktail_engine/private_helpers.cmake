# Internal helpers
# -----------------------------------------------------------------------------

function(_ck_state_append property)
    set_property(GLOBAL APPEND PROPERTY ${property} ${ARGN})
endfunction()

function(_ck_state_set property)
    set_property(GLOBAL PROPERTY ${property} ${ARGN})
endfunction()

function(_ck_state_get out_var property)
    get_property(value GLOBAL PROPERTY ${property})
    set(${out_var} "${value}" PARENT_SCOPE)
endfunction()

# The target a name really refers to. Properties cannot be read from an alias, and
# most of what the framework is handed is one: a module is named through its
# qualified alias, a vendored library through whatever alias its own build publishes.
function(_ck_real_target out_var target)
    get_target_property(aliased ${target} ALIASED_TARGET)
    if(aliased)
        set(${out_var} "${aliased}" PARENT_SCOPE)
    else()
        set(${out_var} "${target}" PARENT_SCOPE)
    endif()
endfunction()

# An empty file the framework offers rather than requires: a module or a plugin gets
# the CMakeLists.txt it would otherwise have to create before it has anything to say.
#
# Only created, never touched. file(TOUCH) moves the date of a file that is already
# there, and a CMakeLists.txt dated after the build system it was read to generate is
# a build system CMake regenerates: doing it from the configure run means a
# reconfigure before every build, for good, with every project file rewritten and
# every custom command it carries looking new.
function(_ck_declare_file path)
    if(NOT EXISTS "${path}")
        file(TOUCH "${path}")
    endif()
endfunction()

# Git tracks files, not directories, so a directory the framework creates empty would
# not survive a clone. The marker is only there to hold the directory: it goes in when
# the directory has nothing else, and is never put back once the directory is used and
# the author has dropped it.
function(_ck_keep_directory directory)
    file(MAKE_DIRECTORY "${directory}")

    set(gitkeep "${directory}/.gitkeep")
    if(NOT EXISTS "${gitkeep}")
        file(TOUCH "${gitkeep}")
    endif()
endfunction()


function(_ck_ignore_directory directory)
    file(MAKE_DIRECTORY "${directory}")

    set(gitignore "${directory}/.gitignore")
    if(NOT EXISTS "${gitignore}")
        set(ignore_content "*\n")
        string(APPEND ignore_content "!.gitignore\n")

        file(WRITE "${gitignore}" "${ignore_content}")
    endif()
endfunction()

# -----------------------------------------------------------------------------
# Platform tags
#
# Platform code sits in a directory named after the tag it belongs to, right next
# to the subsystem it specialises: System/Library/Win32, System/Network/Unix,
# System/Window/Xlib. Nothing else marks it, so an implementation stays with the
# interface it implements rather than in a platform tree of its own, and it keeps
# being reached by the include its directory spells out.
#
# Tags nest, and a file is compiled only when every tag on its path was selected:
# System/Unix/Xlib reads as Unix and Xlib. That is how a backend belonging to one
# family is said without repeating the family on every file.
#
# Two kinds of tags share the one set, deliberately. Win32 and Unix follow from
# the operating system; Xlib and Pthread name a backend the platform offers a
# choice of. What a build selects is therefore a set of tags and not a platform
# name, which is what lets a Wayland backend arrive as one more tag instead of
# another branch threaded through the framework.
#
# The set is closed: a directory whose name is not a tag is ordinary code and is
# always compiled, so Detail/ or Vulkan/ stay out of the mechanism entirely. What
# that costs is a misspelled tag being compiled everywhere in silence, hence the
# aliases below. A spelling that could only ever have been meant as a tag is
# refused by name instead of being taken for a normal directory.
# -----------------------------------------------------------------------------

function(_ck_platform_known_tags out_var)
    set(${out_var} "Win32;Unix;Xlib;Pthread" PARENT_SCOPE)
endfunction()

# Not tags, but nothing else would be written as one. Each maps to what was meant,
# so the error can name it.
function(_ck_platform_tag_aliases out_var)
    set(${out_var}
        "windows=Win32" "win=Win32" "win64=Win32" "msw=Win32"
        "linux=Unix" "gnulinux=Unix" "posix=Unix"
        "x11=Xlib" "xorg=Xlib"
        "pthreads=Pthread"
        PARENT_SCOPE)
endfunction()

# Tags a build starts with. The system decides the family; a backend is only a
# default here, since picking one is the user's call and not the platform's.
function(_ck_default_platform_tags out_var)
    if(WIN32)
        set(${out_var} "Win32" PARENT_SCOPE)
    elseif(APPLE)
        # No windowing backend for this family yet, so the tag it would carry does
        # not exist to be selected.
        set(${out_var} "Unix;Pthread" PARENT_SCOPE)
    elseif(UNIX)
        set(${out_var} "Unix;Pthread;Xlib" PARENT_SCOPE)
    else()
        message(FATAL_ERROR
            "Cocktail Engine: no platform tags known for system '${CMAKE_SYSTEM_NAME}'. "
            "Set CK_PLATFORM_TAGS to the tags it provides.")
    endif()
endfunction()

# Tags in effect: what the user asked for, or the default for the system. Read
# from the cache rather than from state, so that it answers the same before and
# after ck_project.
function(_ck_platform_tags out_var)
    if(DEFINED CK_PLATFORM_TAGS AND NOT CK_PLATFORM_TAGS STREQUAL "")
        set(tags "${CK_PLATFORM_TAGS}")
    else()
        _ck_default_platform_tags(tags)
    endif()

    _ck_platform_known_tags(known)
    foreach(tag IN LISTS tags)
        if(NOT "${tag}" IN_LIST known)
            message(FATAL_ERROR
                "CK_PLATFORM_TAGS: '${tag}' is not a platform tag. Known tags are ${known}.")
        endif()
    endforeach()

    set(${out_var} "${tags}" PARENT_SCOPE)
endfunction()

# Drops the files belonging to a tag this build did not select. Paths are made
# relative to the globbed root first: a tag is a directory the module owns, and a
# segment of the absolute path leading down to the project has no say in it.
function(_ck_select_platform_sources out_var root)
    _ck_platform_known_tags(known)
    _ck_platform_tags(active)
    _ck_platform_tag_aliases(aliases)

    # Directory names are compared folded, so win32/ is the Win32 tag and a
    # misspelling is caught whichever case it was written in.
    set(known_folded "")
    foreach(tag IN LISTS known)
        string(TOLOWER "${tag}" folded)
        list(APPEND known_folded "${folded}")
    endforeach()

    set(active_folded "")
    foreach(tag IN LISTS active)
        string(TOLOWER "${tag}" folded)
        list(APPEND active_folded "${folded}")
    endforeach()

    set(alias_names "")
    foreach(alias IN LISTS aliases)
        string(REGEX REPLACE "=.*$" "" alias_name "${alias}")
        list(APPEND alias_names "${alias_name}")
    endforeach()

    set(selected "")
    foreach(file IN LISTS ARGN)
        get_filename_component(directory "${file}" DIRECTORY)
        file(RELATIVE_PATH relative "${root}" "${directory}")

        set(keep TRUE)
        string(REPLACE "/" ";" segments "${relative}")
        foreach(segment IN LISTS segments)
            string(TOLOWER "${segment}" folded)

            if(NOT folded IN_LIST known_folded)
                if(folded IN_LIST alias_names)
                    list(FIND alias_names "${folded}" index)
                    list(GET aliases ${index} alias)
                    string(REGEX REPLACE "^.*=" "" canonical "${alias}")
                    message(FATAL_ERROR
                        "Cocktail Engine: '${segment}' in ${root}/${relative} is not a "
                        "platform tag, so its files would be compiled on every platform. "
                        "Name the directory '${canonical}'.")
                endif()
                continue()
            endif()

            if(NOT folded IN_LIST active_folded)
                set(keep FALSE)
                break()
            endif()
        endforeach()

        if(keep)
            list(APPEND selected "${file}")
        endif()
    endforeach()

    set(${out_var} "${selected}" PARENT_SCOPE)
endfunction()

# Collects the sources of a module or a game, from the directories they are known to
# live in rather than from everything below. A module is globbed through its two
# source directories, which leaves the rest of its directory to what is not its own
# code: its resources, and the third-party libraries it vendors.
#
# CONFIGURE_DEPENDS avoids having to re-run cmake by hand, at the cost of checking
# the glob on every build.
#
# The glob is deliberately blind to platforms, and what it brings back is sifted
# right after: a module lists no file and names no platform, it only puts its
# platform code in a directory saying which one it is.
function(_ck_collect_sources out_var)
    set(sources "")

    foreach(directory IN LISTS ARGN)
        file(GLOB_RECURSE found CONFIGURE_DEPENDS
            "${directory}/*.cpp"
            "${directory}/*.hpp"
        )
        _ck_select_platform_sources(selected "${directory}" ${found})
        list(APPEND sources ${selected})
    endforeach()

    set(${out_var} "${sources}" PARENT_SCOPE)
endfunction()

# Walks the link graph transitively, resolving aliases. Used to know which
# modules are actually reachable from a game, hence which properties apply to it.
function(_ck_transitive_targets out_var root)
    set(pending "${root}")
    set(visited "")

    while(pending)
        list(POP_FRONT pending current)
        if(NOT TARGET ${current})
            continue()
        endif()
        get_target_property(aliased ${current} ALIASED_TARGET)
        if(aliased)
            set(current "${aliased}")
        endif()
        if(current IN_LIST visited)
            continue()
        endif()
        list(APPEND visited "${current}")

        set(dependencies "")
        get_target_property(link_libraries ${current} LINK_LIBRARIES)
        if(link_libraries)
            list(APPEND dependencies ${link_libraries})
        endif()
        get_target_property(interface_libraries ${current} INTERFACE_LINK_LIBRARIES)
        if(interface_libraries)
            list(APPEND dependencies ${interface_libraries})
        endif()

        foreach(dependency IN LISTS dependencies)
            # Generator expressions cannot be resolved at configure time.
            if(NOT dependency MATCHES "^\\$<")
                list(APPEND pending "${dependency}")
            endif()
        endforeach()
    endwhile()

    set(${out_var} "${visited}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
# Address sanitizer
#
# ENABLE_ASAN builds the code of this project under the address sanitizer, so that
# a use after free, an overrun or a leak becomes a report naming the line it
# happened on instead of whatever the corrupted memory ends up doing later:
#
#   cmake -B build -DENABLE_ASAN=ON
#
# It reaches what the project builds of its own: the modules, the games, the test
# binaries and the library holding what they embed, and stops there. A vendored
# library keeps its own build, for the reason it keeps its own compile
# options: what a third-party build makes of a flag is its business. One of them
# is the shader compiler, which this build runs as a tool, and instrumenting it
# would mean every build needs the sanitizer runtime beside it to compile a shader.
#
# That mix is the one that works. An instrumented module folds an uninstrumented
# archive in and merely loses the checks on that archive's own code; an
# uninstrumented binary linking instrumented objects is what fails to resolve the
# sanitizer runtime, and it is what this arrangement never produces.
#
# It only holds once the standard library has been told that the mix is intended,
# which is what _ck_sanitizer_uniform_definitions is about: MSVC refuses the two
# halves in one binary unless they agree on how a container is annotated, and that
# agreement is a decision of the build rather than of a target.
#
# What the sanitizer reaches is narrower than the instrumentation suggests, and worth
# knowing before reading a report. The engine replaces the global allocation
# operators, and Ck::Memory routes them to BinnedAllocator, which carves its slabs
# out of VirtualAlloc and mmap. The sanitizer intercepts neither, so an engine
# allocation is memory it has no record of: no redzone around it, no poison over it
# once it is freed, and nothing of it in a leak report. An overrun or a use after
# free on engine memory is therefore not caught here, and what the build still gives
# is the checks that do not depend on the allocator, the stack and the globals among
# them, plus whatever an uninstrumented dependency allocates through malloc.
#
# It is also why _ck_sanitizer_uniform_definitions turns the container annotations
# off rather than reconciling them: they are written on the assumption that the
# allocator behind them is the sanitizer's, and here it is not.
# -----------------------------------------------------------------------------

function(_ck_sanitizer_enabled out_var)
    if(ENABLE_ASAN)
        set(${out_var} TRUE PARENT_SCOPE)
    else()
        set(${out_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# How the sanitizer is spelled for the compiler in use. An empty answer is not one:
# a build that asked for the sanitizer and silently got none is worse than a build
# that stops, since the report is the whole reason it was turned on.
function(_ck_sanitizer_options out_compile out_link)
    if(MSVC)
        # clang-cl accepts the same spelling, so this covers both drivers. Nothing is
        # needed at link time: an instrumented object names the runtime it wants
        # itself, through the directives the compiler embeds in it.
        set(compile_options "/fsanitize=address")
        # Incremental linking and the sanitizer are incompatible, and incremental
        # linking is what CMake asks for in the configurations carrying debug
        # information. ck_project takes the contradicting flag out of those, so all
        # that is left here is to say which way it goes.
        set(link_options "/INCREMENTAL:NO")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        # -fno-omit-frame-pointer instruments nothing, it is what makes the report
        # readable: without it the stack the sanitizer walks stops at the first frame
        # that reused the register.
        set(compile_options "-fsanitize=address" "-fno-omit-frame-pointer")
        # The same flag at link time, which is what pulls the runtime in.
        set(link_options "-fsanitize=address")
    else()
        message(FATAL_ERROR
            "ENABLE_ASAN: no address sanitizer flag known for compiler "
            "'${CMAKE_CXX_COMPILER_ID}'. Turn it off, or add the spelling it wants to "
            "_ck_sanitizer_options.")
    endif()

    set(${out_compile} "${compile_options}" PARENT_SCOPE)
    set(${out_link} "${link_options}" PARENT_SCOPE)
endfunction()

# What every object of a sanitizer build has to agree on, whether it is instrumented
# or not.
#
# The MSVC standard library annotates the memory a std::string or a std::vector has
# allocated but not filled yet, so that reading past the size of a container is a
# report instead of a read of memory the container does legitimately own. Whether it
# does changes the inline code a translation unit compiles, so the compiler states
# the answer in every object as a directive the linker checks, and two objects that
# disagree end the link:
#
#   error LNK2038: mismatch detected for 'annotate_string':
#                  value '0' does not match value '1'
#
# This build turns the annotations off, and does so everywhere, which settles both
# the link and a correctness problem they have here.
#
# The correctness problem is that the annotations describe a heap the sanitizer does
# not own. std::allocator tells the standard library, through
# _Minimum_asan_allocation_alignment, that its blocks come from the sanitizer
# allocator: a container may then poison the unused tail of its buffer and count on
# free() to wipe those shadow bytes when the block goes back. The global operators
# this build replaces route to Ck::Memory, whose allocator takes its slabs from
# VirtualAlloc and mmap, which the sanitizer never sees. Nothing wipes the shadow of
# a block the engine recycles, so the poison a container left behind outlives it, and
# the next tenant of the block is reported the first time it writes to memory it does
# legitimately own:
#
#   ERROR: AddressSanitizer: unknown-crash on address 0x...
#   WRITE of size 1 ...
#   Address 0x... is a wild pointer inside of access range of size 0x1
#
# "Wild pointer" is the sanitizer saying the address belongs to no allocation it
# knows of, which is the whole of it: the report comes from a shadow byte written by
# the standard library about memory the runtime has no record of.
#
# _DISABLE_STL_ANNOTATION is the standard library's own switch for it, and it settles
# the link on the way: an object that inserts no annotation states value '0', which
# is what an uninstrumented archive states too, so the two halves agree instead of
# merely declining to answer.
#
# Set for the whole build and not per target, vendored libraries and tools included:
# what it decides is the shape of the inline code of a container, and two objects of
# one program reading it differently is the very thing the check exists to catch.
function(_ck_sanitizer_uniform_definitions out_var)
    if(MSVC)
        set(${out_var} "_DISABLE_STL_ANNOTATION" PARENT_SCOPE)
    else()
        # libstdc++ and libc++ annotate on what the compiler was told and neither
        # cross-checks it at link time, so there is nothing to agree on here.
        # libstdc++ has no container annotation to turn off. libc++ does, and it is
        # opted out of per allocator from C++ rather than from the command line, so
        # the day this build targets libc++ the switch belongs in the code and not
        # here.
        set(${out_var} "" PARENT_SCOPE)
    endif()
endfunction()

# What the sanitizer cannot be built beside, taken out of the flags CMake sets for
# the configurations.
#
# MSVC refuses /RTC1 and /fsanitize=address on the same command line, and /RTC1 is
# in the debug flags CMake writes. Its runtime checks are a subset of what the
# sanitizer does, so dropping them costs nothing.
#
# Incremental linking is likewise refused, and likewise asked for by CMake. It is
# removed rather than contradicted: a linker handed both /INCREMENTAL and
# /INCREMENTAL:NO leaves the outcome to the order they happened to appear in.
#
# Returned as a value rather than set, since a function cannot reach the scope its
# own caller was called from. ck_project is what publishes the result, into the
# directory scope every target of the build reads it from.
function(_ck_sanitizer_strip_conflicts out_var flags)
    string(REGEX REPLACE "(^|[ \t])/RTC[1csu]+" "\\1" flags "${flags}")
    string(REGEX REPLACE "(^|[ \t])/INCREMENTAL(:(YES|NO))?" "\\1" flags "${flags}")
    string(STRIP "${flags}" flags)
    set(${out_var} "${flags}" PARENT_SCOPE)
endfunction()

# Builds one target under the sanitizer.
#
# PRIVATE throughout, and deliberately: instrumentation is a property of the code
# compiled here and not of what the target publishes, so it stays out of the
# interface an install exports and a consumer is never made to build the same way.
function(_ck_configure_sanitizer target)
    _ck_sanitizer_enabled(enabled)
    if(NOT enabled)
        return()
    endif()

    get_target_property(type ${target} TYPE)
    if(type STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()

    _ck_sanitizer_options(compile_options link_options)

    target_compile_options(${target} PRIVATE ${compile_options})
    if(link_options)
        target_link_options(${target} PRIVATE ${link_options})
    endif()
endfunction()

# -----------------------------------------------------------------------------
# Project aggregate
#
# The one target a game links: <Project>::All, an interface library carrying
# every module the project builds. A game therefore names the runtime once instead
# of listing the modules it happens to be made of today, and a module added to the
# project reaches every game without a single CMakeLists.txt to revisit.
#
# Plugin modules stay out. A plugin is opt-in by definition, so a game names the
# ones it wants in DEPENDS, exactly as it does for a third-party library.
#
# When the project consumes an installed engine rather than being one, that engine
# joins the aggregate too: what a game links is the whole runtime, wherever each
# half of it was built.
# -----------------------------------------------------------------------------

function(_ck_project_aggregate out_target out_alias)
    _ck_state_get(project CK_PROJECT_NAME)
    set(${out_target} "${project}-All" PARENT_SCOPE)
    set(${out_alias} "${project}::All" PARENT_SCOPE)
endfunction()

# The engine as something already built: an installed engine this project links,
# and not the one it may be building itself. The aggregate a project declares
# carries the very name an installed engine would when that project is the engine,
# so what answers the question is who declared the target, not that it exists.
# TODO drop this once the engine export is in place: an engine will then always be
# something already built, and the branch it guards goes away with it.
function(_ck_installed_engine out_var)
    set(${out_var} "" PARENT_SCOPE)

    if(NOT TARGET CocktailEngine::All)
        return()
    endif()

    _ck_state_get(generate_aggregate CK_PROJECT_GENERATE_AGGREGATE)
    if(generate_aggregate)
        _ck_state_get(aggregate_target CK_PROJECT_AGGREGATE_TARGET)
        if(aggregate_target STREQUAL "CocktailEngine::All")
            return()
        endif()
    endif()

    set(${out_var} "CocktailEngine::All" PARENT_SCOPE)
endfunction()

function(_ck_declare_project_aggregate)
    _ck_project_aggregate(target alias)

    add_library(${target} INTERFACE)
    add_library(${alias} ALIAS ${target})
    # What the aggregate is called once installed. The real target carries the project
    # in its name to stay unique in a build tree, the exported one does not need to:
    # the namespace of the export set already says which project it comes from, so a
    # consumer finds back the very <Project>::All it would have built.
    set_target_properties(${target} PROPERTIES EXPORT_NAME "All")
    _ck_state_set(CK_PROJECT_AGGREGATE_TARGET "${alias}")

    _ck_installed_engine(installed)
    if(installed)
        target_link_libraries(${target} INTERFACE ${installed})
    endif()
endfunction()

# Adds a module to what the games of this project link. Called for every module the
# project declares itself, and for none of the ones a plugin brings.
#
# The link list of an interface library is resolved at generate time, so a module
# declared after a game still reaches it and modules can be declared in whatever
# order a project likes.
function(_ck_join_project_aggregate module)
    _ck_state_get(generate_aggregate CK_PROJECT_GENERATE_AGGREGATE)
    if(NOT generate_aggregate)
        return()
    endif()

    _ck_project_aggregate(target alias)
    if(NOT TARGET ${target})
        return()
    endif()

    target_link_libraries(${target} INTERFACE ${module})
endfunction()

# Settings shared by every Cocktail target: C++ standard, text encoding, engine
# link.
function(_ck_configure_target target)
    get_target_property(type ${target} TYPE)

    if(NOT type STREQUAL "INTERFACE_LIBRARY")
        target_compile_features(${target} PUBLIC cxx_std_20)
    endif()

    # Windows ships two of every function taking text, and the name in the
    # documentation is a macro resolving to one of them: CreateFile is CreateFileA
    # unless told otherwise, and a struct like WNDCLASSEX is the narrow one likewise.
    # The engine has TextChar as wchar_t there and hands the system UTF-16 throughout,
    # so the narrow half is not a fallback it can live with: it would either fail to
    # compile or, worse, read a wide buffer as bytes.
    #
    # UNICODE selects the W half of the Win32 API, _UNICODE does the same for the text
    # mappings of the CRT (_tcslen and the like). Neither implies the other, and a
    # translation unit that got one but not the other is the one combination that
    # silently mixes the two, so both are set together and in one place.
    #
    # PUBLIC, because this is not an implementation detail: the API of the engine is
    # written in TextChar, so whoever compiles against its headers, and whoever
    # reaches the system beside it, has to see the same half of the platform it was
    # built against. Vendored libraries are left alone: what a third-party build makes
    # of the platform is its own business, and it links through a C++ API where the
    # choice no longer shows.
    if(WIN32)
        if(NOT type STREQUAL "INTERFACE_LIBRARY")
            target_compile_definitions(${target} PUBLIC UNICODE _UNICODE NOMINMAX)
        else()
            target_compile_definitions(${target} INTERFACE UNICODE _UNICODE NOMINMAX)
        endif()
    endif()

    # A source file carrying no byte order mark tells MSVC nothing about its encoding,
    # so MSVC falls back to the active code page of the machine doing the compiling.
    # The same file then means different things from one desk to the next: the two
    # bytes UTF-8 writes é as are one character where the code page is UTF-8 and two
    # unrelated ones where it is 1252, and the literal built from them changes length
    # and content without a diagnostic. /utf-8 fixes the source and the execution
    # character set to UTF-8 both, which moves the answer out of the environment and
    # into the build, where it is the same everywhere.
    #
    # PUBLIC for the reason UNICODE is: the API of the engine is written in TextChar,
    # and whoever compiles against its headers has to read a literal the way the
    # engine was built to read it. Vendored libraries keep whatever their own build
    # settled on. Keyed on the compiler and not on the platform, since /utf-8 is an
    # MSVC spelling: clang-cl takes it, a clang or gcc driver on Windows does not.
    if(MSVC)
        if(NOT type STREQUAL "INTERFACE_LIBRARY")
            target_compile_options(${target} PUBLIC /utf-8)
            target_compile_options(${target} PRIVATE /MP) # Force MSVC to build in parallel
        else()
            target_compile_options(${target} INTERFACE /utf-8)
        endif()
    endif()

    # The engine is not packageable yet, so only link it when it is available.
    # TODO make this link unconditional once the engine export is in place.
    _ck_installed_engine(installed)
    if(installed)
        target_link_libraries(${target} PUBLIC ${installed})
    endif()
endfunction()

# Relative path a binary uses to find the shared modules sitting beside it, so
# that a game runs from the output directory without a preset environment.
function(_ck_loader_path out_var)
    if(APPLE)
        set(${out_var} "@loader_path" PARENT_SCOPE)
    else()
        set(${out_var} "$ORIGIN" PARENT_SCOPE)
    endif()
endfunction()

# Puts the modules this project did not build next to the program that loads them.
#
# The engine loads a module by file name, from the directory the running binary sits
# in, which is why a module built here is written straight into the output directory.
# A module found already built sits under the install prefix of whoever shipped it
# instead, and nothing would look for it there: an installed module is therefore
# copied beside the program, so that what the loader sees is the one layout whether
# the module was built here or found.
#
# Called for a program and not for a module: what has to be complete is the directory
# a binary starts from, and it is the program that knows it starts from there.
function(_ck_stage_external_modules target)
    _ck_state_get(modules CK_MODULES)
    _ck_state_get(output_dir CK_OUTPUT_DIR)

    set(staged "")
    foreach(module IN LISTS modules)
        _ck_module_target(module_target "${module}")
        if(NOT module_target OR NOT TARGET ${module_target})
            continue()
        endif()

        _ck_real_target(module_target "${module_target}")
        get_target_property(imported ${module_target} IMPORTED)
        if(NOT imported)
            continue()
        endif()
        # An imported static module is folded into the binary at link time, so there
        # is no file for the loader to find and nothing to copy.
        get_target_property(type ${module_target} TYPE)
        if(NOT type STREQUAL "SHARED_LIBRARY")
            continue()
        endif()

        list(APPEND staged "$<TARGET_FILE:${module_target}>")
    endforeach()

    if(NOT staged)
        return()
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${output_dir}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${staged} "${output_dir}"
        COMMENT "Staging the modules ${target} loads"
        VERBATIM)
endfunction()

# Macros a module publishes its API through:
#
#   COCKTAILENGINE_CORE_API      what a declaration is annotated with
#   COCKTAILENGINE_CORE_BUILD    set while that module is being compiled, and only then
#
# The export header of a module resolves the first to CK_EXPORT_API or
# CK_IMPORT_API depending on whether the second is defined, so one annotation
# reads as dllexport to the module and dllimport to whoever links it. Both are named
# after the qualified module name, uppercased, which is what keeps two modules of the
# same name in different namespaces from sharing a macro.
function(_ck_module_build_macro out_var namespace name)
    string(TOUPPER "${namespace}_${name}_BUILD" macro)
    string(MAKE_C_IDENTIFIER "${macro}" macro)
    set(${out_var} "${macro}" PARENT_SCOPE)
endfunction()

# What a shared module needs on top of a static one. The binary is written next to
# the games rather than in the build tree: a DLL is only found beside the
# executable that loads it, and the engine loads modules from there.
function(_ck_configure_shared_module target namespace name)
    _ck_state_get(output_dir CK_OUTPUT_DIR)
    _ck_loader_path(loader_path)

    set_target_properties(${target} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${output_dir}"   # .so / .dylib
        RUNTIME_OUTPUT_DIRECTORY "${output_dir}"   # .dll
        BUILD_RPATH "${loader_path}"
        INSTALL_RPATH "${loader_path}"
        # Import libraries stay a build artifact, they are not shipped, so
        # ARCHIVE_OUTPUT_DIRECTORY is left in the build tree.
    )

    # Which side of the shared build this translation unit is on. PRIVATE, and that
    # is the whole point: the module compiles with it and exports, a consumer
    # compiles without it and imports, from the same annotation in the same header.
    #
    # The alternative would be WINDOWS_EXPORT_ALL_SYMBOLS, which spares the
    # annotations by exporting everything. It also exports everything: nothing can
    # be kept internal, the export table carries every symbol the module happens to
    # have, and Windows ends up behaving unlike the platforms where visibility is
    # opt-in. Data symbols are not covered by it either, so the escape hatch is
    # needed anyway the moment a module publishes one.
    _ck_module_build_macro(build_macro "${namespace}" "${name}")
    target_compile_definitions(${target} PRIVATE ${build_macro})
endfunction()





# =============================================================================
# Modules
#
# A module is the compilation unit of the framework, and the only thing that owns
# properties and resources. Belonging to a plugin is one possibility, not a
# requirement: the engine declares its own modules with the same command, and a
# project can add modules next to its games rather than wrapping them in a plugin
# it would be the only consumer of.
#
# What the scope of the call decides is the namespace, and where sources are read
# from:
#   inside a plugin added by ck_add_plugin   <Plugin>::<Module>, plugin sources/
#   at project level                         <Project>::<Module>, project sources/
#
# A module is always referenced in its qualified form, exactly like a property, so
# a module added by a project can never collide with an engine or a plugin one.
# =============================================================================

function(_ck_module_key out_var module)
    string(MAKE_C_IDENTIFIER "${module}" key)
    set(${out_var} "${key}" PARENT_SCOPE)
endfunction()

# Namespace owning whatever is declared in the current scope: the plugin being
# added, or the project itself outside of any plugin.
function(_ck_current_namespace out_var)
    if(CK_CURRENT_PLUGIN)
        set(${out_var} "${CK_CURRENT_PLUGIN}" PARENT_SCOPE)
        return()
    endif()
    _ck_state_get(project_name CK_PROJECT_NAME)
    set(${out_var} "${project_name}" PARENT_SCOPE)
endfunction()

# Directories the current scope reads from, following the same rule.
function(_ck_current_sources_dir out_var)
    if(CK_CURRENT_PLUGIN)
        set(${out_var} "${CK_CURRENT_SOURCES_DIR}" PARENT_SCOPE)
        return()
    endif()
    _ck_state_get(sources_dir CK_SOURCES_DIR)
    set(${out_var} "${sources_dir}" PARENT_SCOPE)
endfunction()

function(_ck_current_resources_dir out_var)
    if(CK_CURRENT_PLUGIN)
        set(${out_var} "${CK_CURRENT_RESOURCES_DIR}" PARENT_SCOPE)
        return()
    endif()
    _ck_state_get(resources_dir CK_RESOURCES_DIR)
    set(${out_var} "${resources_dir}" PARENT_SCOPE)
endfunction()

# Real target of a declared module, empty for an external one and for a module
# that has not been declared yet.
function(_ck_module_target out_var module)
    _ck_module_key(key "${module}")
    _ck_state_get(target CK_MODULE_${key}_TARGET)
    set(${out_var} "${target}" PARENT_SCOPE)
endfunction()

function(_ck_module_declared out_var module)
    _ck_module_key(key "${module}")
    _ck_state_get(name CK_MODULE_${key}_NAME)
    if(name)
        set(${out_var} TRUE PARENT_SCOPE)
    else()
        set(${out_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# A module is a class, <Module>Module, published as
# <Namespace>/<Module>/<Module>Module.hpp. That class is where module-specific code
# goes: what the module does on load, what it exposes to the game, where it reads
# the assets it ships.
#
# Every module is a Module: the engine holds the class the whole framework talks
# to a module through, and what the build knows about a given one is generated as
# an intermediate class implementing that interface. The facts therefore read as
# members of the module class itself rather than as a separate blob to look up, a
# module never spells out a path, and changing the asset layout stays a framework
# decision instead of a search-and-replace across every module.
#
# The generated base is public: the module class is part of the module API, so
# whoever includes it must be able to see what it derives from. Nothing collides,
# since both names are built from the module name.
#
# The strings are returned by overrides rather than exposed as constants: the
# engine reaches a module through a Module reference, so what it knows about
# itself has to be reachable virtually. They are defined in a generated .cpp so
# that the literals live in the module binary and changing one recompiles that
# single translation unit instead of everything including the header.

# Header declaring the base class of every module, and the class it declares.
# Only generated code names them, so the framework can move the header without
# any module noticing.
function(_ck_module_base_class out_class out_header)
    set(${out_class} "Module" PARENT_SCOPE)
    # Published by the Core module of the engine, hence the engine namespace: the
    # engine declares its modules with ck_add_module like everyone else.
    set(${out_header} "CocktailEngine/Core/Module.hpp" PARENT_SCOPE)
endfunction()

# Whether the engine takes part in this build. It is not packageable yet, so a
# project can be configured without it, and generated code cannot include an
# engine header that is nowhere to be found.
# TODO drop this, along with the conditional link in _ck_configure_target, once
# the engine export is in place.
function(_ck_engine_available out_var)
    _ck_installed_engine(installed)
    if(installed)
        set(${out_var} TRUE PARENT_SCOPE)
    else()
        set(${out_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(_ck_generate_module_base target namespace name)
    _ck_module_base_class(module_class module_header)
    _ck_engine_available(engine_available)

    # Same subdirectory the cooking commands and the manifest use.
    set(subdirectory "modules/${namespace}/${name}")
    set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(base "${name}ModuleGenerated")
    # Laid out like the sources it is included from, namespace included, so the
    # generated header is one more <Namespace>/<Module>/ header to whoever reads it
    # and two modules of the same name in different namespaces cannot overwrite
    # each other.
    set(header "${generated_dir}/${namespace}/${name}/${base}.hpp")
    set(source "${generated_dir}/${namespace}/${name}/${base}.cpp")

    # Without the engine there is no interface to derive from, so the generated
    # base stands on its own and declares the very same functions virtual. A
    # module class is written the one way whether the engine is there or not: its
    # overrides keep overriding something.
    if(engine_available)
        set(base_include "\n#include <${module_header}>\n")
        set(base_clause " : public ${module_class}")
        set(virtual_ "")
        set(override_ " override")
    else()
        set(base_include "")
        set(base_clause "")
        set(virtual_ "virtual ")
        set(override_ "")
    endif()

    # Built as one string rather than a list of lines: a ';' ending a C++
    # statement would be read as a list separator and disappear.
    set(header_content "#pragma once\n")
    string(APPEND header_content
        "\n// Generated by Cocktail for ${namespace}::${name}. Do not edit.\n")
    string(APPEND header_content "${base_include}")
    string(APPEND header_content "\n#include <CocktailEngine/Core/String.hpp>\n")
    string(APPEND header_content
        "\n/// Base class of ${name}Module, holding what the build knows\n")
    string(APPEND header_content "/// about the ${namespace}::${name} module.\n")
    string(APPEND header_content "class ${base}${base_clause}\n{\npublic:\n")
    if(NOT engine_available)
        # Stands in for what the interface would bring: a module is only ever held
        # and driven through a base reference.
        string(APPEND header_content "    /// Lets a module be destroyed through a base reference.\n")
        string(APPEND header_content "    virtual ~${base}() = default;\n")
        string(APPEND header_content
            "\n    /// Brings the module up. Called once, after it is loaded.\n")
        string(APPEND header_content "    virtual void Startup() = 0;\n")
        string(APPEND header_content
            "\n    /// Tears the module down. Called once, before it is unloaded.\n")
        string(APPEND header_content "    virtual void Shutdown() = 0;\n\n")
    endif()
    string(APPEND header_content "    /// Bare name of the module.\n")
    string(APPEND header_content "    ${virtual_}Ck::StringView GetName() const${override_};\n")
    string(APPEND header_content
        "\n    /// Name the game and the engine refer to this module by.\n")
    string(APPEND header_content
        "    ${virtual_}Ck::StringView GetQualifiedName() const${override_};\n")
    string(APPEND header_content
        "\n    /// Directory holding the cooked resources of this module, as the\n")
    string(APPEND header_content
        "    /// manifest spells it: relative to wherever the resources of the\n")
    string(APPEND header_content "    /// game are rooted.\n")
    string(APPEND header_content
        "    ${virtual_}Ck::StringView GetResourcesSubdirectory() const${override_};\n")
    string(APPEND header_content "};\n")

    set(source_content "// Generated by Cocktail for ${namespace}::${name}. Do not edit.\n")
    string(APPEND source_content "\n#include <${namespace}/${name}/${base}.hpp>\n")
    string(APPEND source_content "\nCk::StringView ${base}::GetName() const\n{\n")
    string(APPEND source_content "    return CK_TEXT(\"${name}\");\n}\n")
    string(APPEND source_content "\nCk::StringView ${base}::GetQualifiedName() const\n{\n")
    string(APPEND source_content "    return CK_TEXT(\"${namespace}::${name}\");\n}\n")
    string(APPEND source_content
        "\nCk::StringView ${base}::GetResourcesSubdirectory() const\n{\n")
    string(APPEND source_content "    return CK_TEXT(\"${subdirectory}\");\n}\n")

    # file(GENERATE) leaves the file alone when the content has not changed, so
    # reconfiguring does not rebuild every module.
    file(GENERATE OUTPUT "${header}" CONTENT "${header_content}")
    file(GENERATE OUTPUT "${source}" CONTENT "${source_content}")

    target_sources(${target} PRIVATE "${header}" "${source}")
    # The build tree only: the generated header is laid out under <Namespace>/<Module>/
    # exactly as the published ones are, so once installed it sits among them and the
    # include root the module already names covers it.
    target_include_directories(${target} PUBLIC "$<BUILD_INTERFACE:${generated_dir}>")

    # Part of the module API, hence part of what an install ships. Recorded rather
    # than installed here: whether this project installs anything at all is settled
    # once, at the end of the configuration.
    _ck_module_key(key "${namespace}::${name}")
    _ck_state_append(CK_MODULE_${key}_PUBLIC_GENERATED "${header}")
endfunction()

# Where the global operators route their allocations, and the header declaring it.
# Empty when nothing provides it, which is a build without the engine: there is no
# allocator to route to, and the operators are simply left as the standard library
# has them.
# TODO collapse this to the engine target alone once the engine export is in place.
function(_ck_memory_provider out_target out_header)
    # Published by the Core module of the engine, like the module interface above.
    set(${out_header} "CocktailEngine/Core/Memory/Memory.hpp" PARENT_SCOPE)

    _ck_installed_engine(installed)
    if(installed)
        set(${out_target} "${installed}" PARENT_SCOPE)
    elseif(TARGET CocktailEngine::Core)
        # The engine building itself: the module publishing the allocator is right
        # there, the aggregate that will ship it is not.
        set(${out_target} "CocktailEngine::Core" PARENT_SCOPE)
    else()
        set(${out_target} "" PARENT_SCOPE)
    endif()
endfunction()

# Module holding the entry point of a program: the main() the platform hands control
# to, which boots the application and calls the ApplicationMain the program provides.
# A test binary is a program like a game is, so it gets its entry point from there
# rather than from the test framework.
#
# Returned as a name and not checked against the declared targets: modules are
# declared in whatever order a project likes, and a test may well be added before
# this one is. What it names has to exist by the end of the configuration, which is
# when CMake resolves it, and says so itself if it does not.
# TODO collapse this to the engine target once the engine export is in place.
function(_ck_entry_point_provider out_target)
    set(${out_target} "CocktailEngine::Main" PARENT_SCOPE)
endfunction()

# Global operator new and delete, routed to the engine allocator.
#
# Replacing them is a per-binary affair and not a per-project one. Each shared
# module holds its own copy of the operators, and on Windows its own CRT heap
# behind them, so a module that did not replace them allocates from that heap
# while the engine frees from its allocator. Written by hand it would be the same
# hundred lines copied into every module, saying nothing about the module it sits
# in, so it is generated next to the module base instead: a module gets the engine
# allocator for being a module, and the day the operators need to change, they
# change in one place.
#
# Static modules are left out. They are folded into a binary that replaces the
# operators itself, and two archives each defining them would collide at link
# time.
function(_ck_generate_memory_overrides target namespace name)
    _ck_memory_provider(provider memory_header)
    if(NOT provider)
        return()
    endif()

    # Same directory as the module base, laid out the same way, so what the build
    # writes for a module is all in one place.
    set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(source "${generated_dir}/${namespace}/${name}/${name}NewGenerated.cpp")

    set(content "// Generated by Cocktail for ${namespace}::${name}. Do not edit.\n")
    string(APPEND content
        "\n// Global allocation operators, routed to the engine allocator. Replaced in\n")
    string(APPEND content "// every shared module, so that whoever allocates in this binary and\n")
    string(APPEND content "// whoever frees in another agree on the heap.\n")
    string(APPEND content "\n#include <cstddef>\n")
    string(APPEND content "#include <new>\n")
    string(APPEND content "\n#include <${memory_header}>\n")
    # A literal from here on: nothing below depends on the module, and a bracket
    # argument keeps the C++ readable as C++, semicolons included.
    string(APPEND content [==[

void* operator new(std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new[](std::size_t size, std::align_val_t alignment)
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new[](std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void operator delete(void* pointer) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}
]==])

    file(GENERATE OUTPUT "${source}" CONTENT "${content}")

    target_sources(${target} PRIVATE "${source}")

    # The operators call into the module publishing the allocator, so the
    # translation unit brings its own dependency along instead of counting on the
    # module having declared it. PRIVATE: none of it shows up in what the module
    # publishes. Skipped for the provider itself, which cannot link to itself.
    get_target_property(provider_target ${provider} ALIASED_TARGET)
    if(NOT provider_target)
        set(provider_target "${provider}")
    endif()
    if(NOT provider_target STREQUAL target)
        target_link_libraries(${target} PRIVATE ${provider})
    endif()
endfunction()

# A module directory is split in two: include/ holds what the module publishes,
# src/ what it keeps to itself. Only include/ is on the include path of the
# consumers, so the header a module means to expose and the one it does not is a
# matter of which directory it sits in, with no export list to maintain.
#
# Both halves nest the files under <Namespace>/<Module>/, so an include reads like
# the qualified name of the module it comes from: <CocktailEngine/Core/CoreModule.hpp>
# from a consumer, <CocktailEngine/Core/Internal/Cache.hpp> from inside. Nothing can
# collide, whoever the module belongs to, and a file keeps the same include whether
# it moves from src/ to include/ or the other way around.
#
# Writes the module class of a brand new module, so that a module always has the
# entry point the engine expects instead of failing on an empty directory. A module
# that already holds sources is left alone: past that point how it fills those two
# directories is the author's business, not the framework's.
#
# Startup and Shutdown come scaffolded, empty: they are the two moments the engine
# hands the module, and having them already written is what makes the module class
# an entry point rather than a declaration to complete.
function(_ck_scaffold_module_class namespace name source_dir)
    set(module_class_name "${name}Module")

    set(header "${source_dir}/include/${namespace}/${name}/${module_class_name}.hpp")
    set(source "${source_dir}/src/${namespace}/${name}/${module_class_name}.cpp")

    if(NOT EXISTS "${header}")
        set(header_content "#pragma once\n")
        string(APPEND header_content
            "\n#include <${namespace}/${name}/${module_class_name}Generated.hpp>\n")
        string(APPEND header_content "\n/// Entry point of the ${namespace}::${name} module.\n")
        string(APPEND header_content "///\n")
        string(APPEND header_content "/// Module-specific code goes here. What the build knows\n")
        string(APPEND header_content "/// about the module comes from the generated base class.\n")
        string(APPEND header_content "class ${module_class_name} : public ${module_class_name}Generated\n")
        string(APPEND header_content "{\npublic:\n")
        string(APPEND header_content "    /// Brings the module up. Called once, after it is loaded.\n")
        string(APPEND header_content "    void Startup() override;\n")
        string(APPEND header_content "\n    /// Tears the module down. Called once, before it is unloaded.\n")
        string(APPEND header_content "    void Shutdown() override;\n")
        string(APPEND header_content "};\n")

        file(WRITE "${header}" "${header_content}")
    endif()

    if(NOT EXISTS "${source}")
        set(source_content "#include <${namespace}/${name}/${module_class_name}.hpp>\n")
        string(APPEND source_content "\nvoid ${module_class_name}::Startup()\n{\n}\n")
        string(APPEND source_content "\nvoid ${module_class_name}::Shutdown()\n{\n}\n")

        file(WRITE "${source}" "${source_content}")
    endif()
endfunction()

# -----------------------------------------------------------------------------
# Debugger visualisers
#
# A .natvis file says how the types of a module are displayed while debugging, and
# it is the module that knows: whoever wrote Array is who can say that what matters
# about it is its size and its elements. A module drops the file in and declares
# nothing, exactly as it does for its sources.
#
# The files are read from the root of the module, for the visualisers of everything
# it publishes, and from its two source halves, for one written next to the type it
# describes. resources/ and thirdparty/ are left out, for the same reason the source
# glob leaves them out: what a vendored library ships is its own business, and it
# would arrive here with visualisers for types no module of ours ever names. Platform
# directories are honoured on the way, so a visualiser for platform code follows the
# code it describes instead of being embedded in a build that never compiled it.
# -----------------------------------------------------------------------------

function(_ck_collect_visualisers out_var module_dir)
    file(GLOB visualisers CONFIGURE_DEPENDS "${module_dir}/*.natvis")

    foreach(directory IN ITEMS "${module_dir}/include" "${module_dir}/src")
        file(GLOB_RECURSE found CONFIGURE_DEPENDS "${directory}/*.natvis")
        _ck_select_platform_sources(selected "${directory}" ${found})
        list(APPEND visualisers ${selected})
    endforeach()

    set(${out_var} "${visualisers}" PARENT_SCOPE)
endfunction()

# Hands a module its visualisers, and hands them to whoever links it.
#
# A natvis is only of use once it sits in the PDB of a binary loaded in the debug
# session, which is what the linker does with /NATVIS. It is therefore attached to
# the interface of the module as much as to the module itself: a game or a test
# binary embeds the visualisers of every module behind it, transitively, without
# naming a single one. Debugging a game shows Ck::Array the way Core describes it
# because the game links Core, and nothing else had to be said.
#
# A static module has no link of its own for the option to land on, so what carries
# its visualisers is the binary it is folded into, which is where they were needed
# anyway.
function(_ck_attach_visualisers target namespace name module_dir)
    _ck_collect_visualisers(visualisers "${module_dir}")
    if(NOT visualisers)
        return()
    endif()

    # Listed among the sources of the module, so that an IDE shows the file with the
    # module owning it and a build notices when it changes. Never built: it is not
    # code, and what it takes part in is the link below. The Visual Studio generator
    # would otherwise turn it into a Natvis item and embed it a second time, hence
    # the tool it is given instead.
    target_sources(${target} PRIVATE ${visualisers})
    set_source_files_properties(${visualisers} PROPERTIES
        HEADER_FILE_ONLY TRUE
        VS_TOOL_OVERRIDE "None")

    # A natvis is read by the Visual Studio debugger and by nothing else, so only a
    # linker that knows what to do with it is told about it.
    if(NOT MSVC)
        return()
    endif()

    # Ignored by the linker when the configuration produces no PDB, so there is no
    # build type to check here: a release build simply has nothing to embed it in.
    #
    # Named twice, like the include root of the module: the file in the source tree
    # while this project builds it, the copy sitting under the install prefix once it
    # has been installed. $<INSTALL_PREFIX> is what makes the second one absolute,
    # since a link option is handed to the linker as written and nothing rewrites a
    # relative path in it the way it does for an include directory.
    #
    # The copy is renamed on the way: a visualiser is often the '.natvis' sitting at
    # the root of a module, so the name it is written under says nothing about which
    # module it came from and two of them would land on the same file.
    _ck_state_get(install_debug_dir CK_INSTALL_DEBUG_DIR)
    set(index 0)
    foreach(visualiser IN LISTS visualisers)
        set(installed_name "${namespace}-${name}-${index}.natvis")
        target_link_options(${target} PUBLIC
            "$<BUILD_INTERFACE:/NATVIS:${visualiser}>"
            "$<INSTALL_INTERFACE:/NATVIS:$<INSTALL_PREFIX>/${install_debug_dir}/${installed_name}>")

        _ck_module_key(key "${namespace}::${name}")
        _ck_state_append(CK_MODULE_${key}_VISUALISERS "${visualiser}")
        _ck_state_append(CK_MODULE_${key}_VISUALISER_NAMES "${installed_name}")

        math(EXPR index "${index} + 1")
    endforeach()
endfunction()





# Body of the entry point of a test binary. Written as a bracket argument, so the
# C++ goes in verbatim: no quote to escape, and a ';' ending a statement stays a ';'
# instead of splitting a list.
#
# This is the one place in the framework that knows the test framework by name, and
# it is where it belongs: what a run is, and how a result becomes an exit code, is
# what a test framework decides. Swapping Catch2 for another one is this function
# and nothing else.
function(_ck_test_runner out_content)
    set(content [==[
#include <catch2/catch_session.hpp>

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>

#include <CocktailEngine/Main/ExitCode.hpp>

using namespace Ck;
using namespace Ck::Main;

COCKTAIL_DECLARE_LOG_CATEGORY(Catch2, LogLevel::Info);
COCKTAIL_DEFINE_LOG_CATEGORY(Catch2);

ExitCode ApplicationMain(Application* application)
{
    Catch::Session session;

#ifndef NDEBUG
    session.configData().verbosity = Catch::Verbosity::High;
#endif

    CK_LOG(Catch2, LogLevel::Info, CK_TEXT("Starting Catch2 test session"));

    // The session is handed the arguments the application was started with, so a test
    // binary takes the options of the framework it was built against.
#ifdef COCKTAIL_OS_WINDOWS
    Array<const TextChar*> arguments = application->GetArgv().Transform([](const String& argument) {
        return argument.GetData();
    });
#else
    Array<const AnsiChar*> arguments = application->GetArgv().Transform([](const String& argument) {
        return reinterpret_cast<const AnsiChar*>(argument.GetData());
    });
#endif

    int parseResult = session.applyCommandLine(static_cast<int>(arguments.GetSize()), arguments.GetData());
    if (parseResult != 0)
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Failed to parse application arguments"));
        return ExitCode::GeneralError;
    }

    int result = session.run();
    if (result == 0)
    {
        CK_LOG(Catch2, LogLevel::Info, CK_TEXT("All tests passed"));
        return ExitCode::Success;
    }

    if (result > 0)
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Test session reported %d failure(s)"), result);
    }
    else
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Test session aborted on an internal error"));
    }

    // A failure has to reach the exit code of the process, otherwise CTest reports the
    // run as passed.
    return ExitCode::GeneralError;
}
]==])

    set(${out_content} "${content}" PARENT_SCOPE)
endfunction()

# Entry point of the test binary of a module, written into its generated directory
# next to what ck_add_module generates for the module itself.
#
# Generated for the same reason the allocation operators are: it is the same file in
# every test binary, it says nothing about the module it belongs to, and the day the
# engine entry point changes shape, it changes here rather than in the tests of every
# module. It is a source of the binary and not of a library, since the symbol it
# defines is only ever referred to from the entry point module, and a linker does not
# go back into an archive it has already walked past to find it.
function(_ck_generate_test_main out_source namespace name)
    _ck_test_runner(runner)

    set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
    set(source "${generated_dir}/${namespace}/${name}/${name}TestMainGenerated.cpp")

    set(content "// Generated by Cocktail for the tests of ${namespace}::${name}. Do not edit.\n")
    string(APPEND content
        "//\n// Runs the Catch2 session from the ApplicationMain the engine entry point\n")
    string(APPEND content "// calls, so a test sees the application a game sees.\n\n")
    string(APPEND content "${runner}")

    file(GENERATE OUTPUT "${source}" CONTENT "${content}")

    set(${out_source} "${source}" PARENT_SCOPE)
endfunction()





# =============================================================================
# Module properties
#
# Every module, whether it belongs to the engine or to a plugin, declares the
# properties it exposes. A game sets their values. A property name is therefore
# owned by its module, so a third-party plugin cannot collide with the engine.
#
# A property is always referenced in its qualified form <Module>::<Property>,
# engine or plugin alike. No short name is ever resolved, which means adding a
# property to a module can never break an existing project.
#
# Values are never resolved against the compiler. The build only declares the
# schema, validates what a game sets against it, and aggregates the effective
# values into the configuration the game reads at startup:
#
#   bin/<Game>.ckconfig.yml
#
# Nothing a property holds ends up in a compile definition, so a module is
# compiled once and stays valid for every game whatever their values are, two
# games of the same project are free to disagree on any property, and changing a
# value is a matter of editing the generated file rather than of rebuilding.
# =============================================================================

function(_ck_property_key out_var module property)
    string(REPLACE "::" "_" identifier "${module}_${property}")
    string(MAKE_C_IDENTIFIER "${identifier}" identifier)
    set(${out_var} "${identifier}" PARENT_SCOPE)
endfunction()

# Resolves a property reference. The qualified form <Module>::<Property> is the
# only one accepted, for the engine and for plugins alike: a short name that is
# unambiguous today would stop being so as soon as another module declares a
# property with the same name, and the user project would break without having
# changed anything.
#
# The short name index is kept, but only to suggest the qualified form in error
# messages.
function(_ck_resolve_property reference out_module out_property)
    if(NOT reference MATCHES "^(.+)::([^:]+)$")
        _ck_state_get(owners CK_PROPERTY_OWNERS_${reference})
        if(owners)
            list(JOIN owners "::${reference}, " suggestions)
            message(FATAL_ERROR
                "ck_set_property: '${reference}' must be qualified with its module.\n"
                "Did you mean ${suggestions}::${reference} ?")
        endif()
        _ck_state_get(known CK_PROPERTIES)
        message(FATAL_ERROR
            "ck_set_property: unknown property '${reference}'. Use the "
            "<Module>::<Property> form.\n"
            "Declared properties: ${known}")
    endif()

    set(module "${CMAKE_MATCH_1}")
    set(property "${CMAKE_MATCH_2}")

    _ck_property_key(key "${module}" "${property}")
    _ck_state_get(declared CK_PROPERTY_${key}_MODULE)
    if(NOT declared)
        _ck_state_get(module_keys CK_PROPERTY_KEYS_${module})
        if(module_keys)
            set(module_properties "")
            foreach(module_key IN LISTS module_keys)
                _ck_state_get(module_property CK_PROPERTY_${module_key}_NAME)
                list(APPEND module_properties "${module_property}")
            endforeach()
            message(FATAL_ERROR
                "ck_set_property: module '${module}' declares no property named "
                "'${property}'.\n"
                "Properties of ${module}: ${module_properties}")
        endif()
        message(FATAL_ERROR
            "ck_set_property: module '${module}' declares no property at all.")
    endif()

    set(${out_module} "${module}" PARENT_SCOPE)
    set(${out_property} "${property}" PARENT_SCOPE)
endfunction()

function(_ck_validate_property_value module property value)
    _ck_property_key(key "${module}" "${property}")
    _ck_state_get(type CK_PROPERTY_${key}_TYPE)
    _ck_state_get(values CK_PROPERTY_${key}_VALUES)

    if(type STREQUAL "ENUM")
        if(NOT value IN_LIST values)
            message(FATAL_ERROR
                "${module}::${property}: invalid value '${value}'. Allowed values: ${values}")
        endif()
    elseif(type STREQUAL "BOOL")
        if(NOT value MATCHES "^(ON|OFF|TRUE|FALSE|YES|NO|0|1)$")
            message(FATAL_ERROR
                "${module}::${property}: expected a boolean, got '${value}'.")
        endif()
    elseif(type STREQUAL "INT")
        if(NOT value MATCHES "^-?[0-9]+$")
            message(FATAL_ERROR "${module}::${property}: expected an integer, got '${value}'.")
        endif()
    endif()
endfunction()







# =============================================================================
# Resources
#
# A resource is always owned by something that ships it: a game, or a module.
# Ownership is what makes the two kinds distinct, and it decides three things at
# once:
#
#   where the raw file is read from   the owner's own resources/ directory
#   where the cooked asset is written a directory reserved for that owner
#   how the game refers to it         plain name for its own, qualified
#                                     <Namespace>::<Module>::<Name> for a module's
#
# The naming rule is the one already used for properties: a game owns its own
# namespace, so its resource names stay bare, while anything coming from a module
# is qualified by it. A module can therefore add a resource without ever shadowing
# a game asset, and two modules cannot collide with each other.
#
# The physical separation matters just as much as the naming one: two games of
# the same project may each declare a 'Sponza' pointing at a different file, and
# a module may ship a 'Models/Sponza' of its own. Cooking into a per-owner
# directory keeps those three assets from overwriting one another.
# =============================================================================

function(_ck_resource_owner_key out_var owner)
    string(MAKE_C_IDENTIFIER "${owner}" key)
    set(${out_var} "${key}" PARENT_SCOPE)
endfunction()

function(_ck_resource_key out_var owner name)
    string(MAKE_C_IDENTIFIER "${owner}_${name}" key)
    set(${out_var} "${key}" PARENT_SCOPE)
endfunction()

# Identifies the owner of a resource from the scope of the call rather than from
# a keyword: the file is read from the resources/ directory of that scope, and the
# namespace it may be qualified with is the one of that scope too. A plugin can
# therefore never declare resources on behalf of someone else.
#
# A plugin only builds modules, so a bare name there is one of them. At project
# level games and modules coexist, so the module registry decides: a bare name
# matching a declared module belongs to that module, anything else is a game. A
# module must consequently be declared before its resources, or be named in its
# qualified form.
function(_ck_resolve_resource_owner target out_owner out_kind out_resources_dir)
    _ck_current_namespace(namespace)
    _ck_current_resources_dir(resources_dir)

    if(NOT resources_dir)
        message(FATAL_ERROR "ck_add_resource: call ck_project() first.")
    endif()

    if(target MATCHES "^(.+)::([^:]+)$")
        # Already qualified: only the namespace of the current scope may be named.
        if(NOT CMAKE_MATCH_1 STREQUAL namespace)
            message(FATAL_ERROR
                "ck_add_resource: '${target}' belongs to '${CMAKE_MATCH_1}'. A resource is "
                "declared by whoever builds the module that ships it, so that it travels "
                "with it.")
        endif()
        set(owner "${target}")
        set(kind MODULE)
    else()
        _ck_module_declared(declared "${namespace}::${target}")
        if(CK_CURRENT_PLUGIN OR declared)
            set(owner "${namespace}::${target}")
            set(kind MODULE)
        else()
            set(owner "${target}")
            set(kind GAME)
        endif()
    endif()

    set(${out_owner} "${owner}" PARENT_SCOPE)
    set(${out_kind} "${kind}" PARENT_SCOPE)
    set(${out_resources_dir} "${resources_dir}" PARENT_SCOPE)
endfunction()

# Target a resource owner attaches its cooking to. Resolved lazily, so that
# resources can be declared before the game or the module they belong to.
function(_ck_resource_owner_target out_var owner kind)
    if(kind STREQUAL "GAME")
        set(${out_var} "${owner}" PARENT_SCOPE)
        return()
    endif()
    _ck_module_target(target "${owner}")
    set(${out_var} "${target}" PARENT_SCOPE)
endfunction()





# Emits the cooking commands of one owner. A module is built once, so its assets
# are cooked once too, whatever the number of games consuming them. The cook
# target is attached to the owner target, which makes the resources of a module
# follow it naturally through the link graph.
function(_ck_cook_resources owner)
    _ck_resource_owner_key(owner_key "${owner}")
    _ck_state_get(kind CK_RESOURCE_OWNER_${owner_key}_KIND)
    _ck_resource_owner_target(target "${owner}" "${kind}")
    _ck_state_get(resources_dir CK_RESOURCE_OWNER_${owner_key}_RESOURCES_DIR)
    _ck_state_get(subdir CK_RESOURCE_OWNER_${owner_key}_SUBDIR)
    _ck_state_get(names CK_RESOURCES_${owner_key})
    _ck_state_get(intermediate_dir CK_INTERMEDIATE_DIR)

    set(cooked_files "")

    foreach(name IN LISTS names)
        _ck_resource_key(key "${owner}" "${name}")
        _ck_state_get(relative_file CK_RESOURCE_${key}_FILE)

        set(source_file "${resources_dir}/${relative_file}")
        set(cooked_file "${intermediate_dir}/${subdir}/${name}.ckasset")
        get_filename_component(cooked_dir "${cooked_file}" DIRECTORY)

        # TODO replace with the engine cooker, exposed as an imported target so
        # that DEPENDS triggers a re-cook when the tool itself changes:
        #   COMMAND CocktailEngine::Cooker --in <src> --out <dst> ${options}
        #   DEPENDS "${source_file}" CocktailEngine::Cooker
        add_custom_command(
            OUTPUT "${cooked_file}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${cooked_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_file}" "${cooked_file}"
            DEPENDS "${source_file}"
            COMMENT "Cooking ${owner} ${name}"
            VERBATIM)

        list(APPEND cooked_files "${cooked_file}")
    endforeach()

    # An external module ships already cooked assets: nothing to attach them to,
    # and nothing to rebuild.
    if(cooked_files AND target)
        add_custom_target(${target}_resources DEPENDS ${cooked_files})
        set_target_properties(${target}_resources PROPERTIES FOLDER "Resources")
        add_dependencies(${target} ${target}_resources)
    endif()
endfunction()

# The manifest is per game, and lists what this game actually ships: its own
# resources plus those of the plugin modules it links. A module cooked for
# another game of the same project stays out of it, which is the same reachability
# rule already used for properties.
function(_ck_write_manifest game)
    _ck_state_get(output_dir CK_OUTPUT_DIR)
    _ck_reachable_modules(modules ${game})

    set(entries "")

    foreach(owner ${game} ${modules})
        _ck_resource_owner_key(owner_key "${owner}")
        _ck_state_get(names CK_RESOURCES_${owner_key})
        if(NOT names)
            continue()
        endif()
        _ck_state_get(namespace CK_RESOURCE_OWNER_${owner_key}_NAMESPACE)
        _ck_state_get(subdir CK_RESOURCE_OWNER_${owner_key}_SUBDIR)

        foreach(name IN LISTS names)
            list(APPEND entries
                "  { \"name\": \"${namespace}${name}\", \"path\": \"${subdir}/${name}.ckasset\", \"owner\": \"${owner}\" }")
        endforeach()
    endforeach()

    list(JOIN entries ",\n" manifest_body)
    file(WRITE "${output_dir}/${game}.manifest.json"
        "{\n \"resources\": [\n${manifest_body}\n ]\n}\n")
endfunction()

# Modules reachable from a game: the ones present in its link graph, plus the
# external ones, which have no target to look for.
function(_ck_reachable_modules out_var game)
    _ck_state_get(modules CK_MODULES)
    _ck_transitive_targets(linked ${game})

    set(reachable "")
    foreach(module IN LISTS modules)
        _ck_module_target(target "${module}")
        if(NOT target)
            # An already built module that came without a target of its own: nothing
            # says whether the game reaches it, so it is taken to. A module found
            # through a package does have one, and follows the link graph like the
            # rest.
            list(APPEND reachable "${module}")
        elseif(target IN_LIST linked)
            list(APPEND reachable "${module}")
        endif()
    endforeach()

    set(${out_var} "${reachable}" PARENT_SCOPE)
endfunction()

# Renders a value as a YAML scalar of the kind its declared type calls for, so
# that the configuration carries types rather than strings: an INT reads back as a
# number, a BOOL as a boolean, and anything textual is quoted. ENUM stays a
# string, which is what the module matches on.
function(_ck_yaml_scalar out_var type value)
    if(type STREQUAL "INT")
        set(scalar "${value}")
    elseif(type STREQUAL "BOOL")
        # The value passed validation, so it is one of the boolean spellings CMake
        # understands. Normalize it, the reader only knows true and false.
        if(value)
            set(scalar "true")
        else()
            set(scalar "false")
        endif()
    else()
        string(REPLACE "\\" "\\\\" escaped "${value}")
        string(REPLACE "\"" "\\\"" escaped "${escaped}")
        set(scalar "\"${escaped}\"")
    endif()
    set(${out_var} "${scalar}" PARENT_SCOPE)
endfunction()

# Effective value of every property reachable from a game: what the game set,
# otherwise the declared default, a property left without either being simply
# absent so the module falls back to whatever it considers its own default.
#
# Written as one string rather than assembled from lists: a value holding a ';'
# would otherwise be read as a list separator and split.
function(_ck_resolve_properties game out_body)
    _ck_reachable_modules(modules ${game})
    set(body "")

    foreach(module IN LISTS modules)
        _ck_state_get(keys CK_PROPERTY_KEYS_${module})
        set(module_body "")

        foreach(key IN LISTS keys)
            _ck_state_get(property CK_PROPERTY_${key}_NAME)
            _ck_state_get(type CK_PROPERTY_${key}_TYPE)
            _ck_state_get(required CK_PROPERTY_${key}_REQUIRED)
            _ck_state_get(value CK_VALUE_${game}_${key})

            if(value STREQUAL "")
                _ck_state_get(value CK_PROPERTY_${key}_DEFAULT)
                if(value STREQUAL "" AND required)
                    message(FATAL_ERROR
                        "${game}: property '${module}::${property}' is required and has "
                        "neither a value nor a default.")
                endif()
            endif()
            if(value STREQUAL "")
                continue()
            endif()

            _ck_yaml_scalar(scalar "${type}" "${value}")
            string(APPEND module_body "    ${property}: ${scalar}\n")
        endforeach()

        if(NOT module_body STREQUAL "")
            # The module name is quoted: '::' is legal in a plain YAML key, but
            # quoting keeps it a single scalar for any reader.
            string(APPEND body "  \"${module}\":\n${module_body}")
        endif()
    endforeach()

    set(${out_body} "${body}" PARENT_SCOPE)
endfunction()

# Aggregates into one file the properties of every module the game reaches,
# grouped by owning module so a name can only ever mean one thing. This is the
# whole configuration surface of the game: read at startup, editable next to the
# binary without a rebuild.
function(_ck_write_config game)
    _ck_state_get(output_dir CK_OUTPUT_DIR)
    _ck_resolve_properties(${game} properties)

    set(content "# Configuration of ${game}, generated by Cocktail.\n")
    string(APPEND content "# Values of the properties declared by the modules this game links.\n")
    string(APPEND content "game: \"${game}\"\n")
    if(properties STREQUAL "")
        string(APPEND content "properties: {}\n")
    else()
        string(APPEND content "properties:\n${properties}")
    endif()

    file(WRITE "${output_dir}/${game}.ckconfig.yml" "${content}")
endfunction()

# =============================================================================
# Install and export
#
# What a project ships is what someone else builds against: the modules, the
# headers they publish, and the CMake package that puts them back together as the
# very targets they were built as. A consumer therefore writes
#
#   find_package(CocktailEngine REQUIRED)
#   ck_project(NAME MyGame)
#   ck_add_game(Sandbox)
#
# and links CocktailEngine::Core, CocktailEngine::All, ImGui::ImGuiIntegration
# under the names they have here, whether they were built beside it or found.
#
# Three things are shipped, and the framework is one of them: an installed engine
# carries its facade as <Project>Macros.cmake and its components under
# <Project>Macros/, so ck_add_module and the rest are the commands the engine was
# built with rather than a copy that drifted. Beside them goes a registry replaying
# what the build knows and no target can carry: which modules exist, and which
# properties they declare.
#
# Nothing here is written by a module. What an install ships follows from what was
# declared: a module is installed for being a module, its published half for being
# under include/, and a vendored library for still being named in the interface of
# something installed.
#
# The install rules are emitted at the end of the configuration rather than as each
# module is declared, because the question they answer is a whole-project one: which
# targets are still reachable from an installed interface, and therefore have to be
# shipped for that interface to resolve.
# =============================================================================

function(_ck_install_enabled out_var)
    _ck_state_get(enabled CK_INSTALL_ENABLED)
    if(enabled)
        set(${out_var} TRUE PARENT_SCOPE)
    else()
        set(${out_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Export set a namespace ships its targets in. One set per namespace, since a set
# carries a single namespace and a plugin module is not the project's to rename:
# what a consumer links is the ImGui::ImGuiIntegration it would have built.
function(_ck_export_set out_var namespace)
    set(${out_var} "${namespace}Targets" PARENT_SCOPE)
endfunction()

# Ships the headers of a directory, keeping the layout they are written in: the
# include root moves, so <CocktailEngine/Core/CoreModule.hpp> stays that include.
#
# Platform code is left out the way the build leaves it out. A header belonging to a
# tag this build did not select describes an implementation that was never compiled,
# so shipping it would offer a declaration nothing stands behind.
function(_ck_install_headers directory destination)
    if(NOT IS_DIRECTORY "${directory}")
        return()
    endif()

    _ck_platform_known_tags(known)
    _ck_platform_tags(active)

    set(excluded "")
    foreach(tag IN LISTS known)
        if(NOT tag IN_LIST active)
            list(APPEND excluded REGEX "(^|/)${tag}(/|$)" EXCLUDE)
        endif()
    endforeach()

    install(DIRECTORY "${directory}/" DESTINATION "${destination}"
        FILES_MATCHING PATTERN "*.hpp" ${excluded})
endfunction()

# Ships one module: the binary, the half of its sources it publishes, the base the
# build generated for it, and whatever it says about how its types are displayed.
function(_ck_install_module module export_set)
    _ck_module_key(key "${module}")
    _ck_state_get(target CK_MODULE_${key}_TARGET)
    _ck_state_get(namespace CK_MODULE_${key}_NAMESPACE)
    _ck_state_get(name CK_MODULE_${key}_NAME)
    _ck_state_get(directory CK_MODULE_${key}_DIRECTORY)

    _ck_state_get(include_dir CK_INSTALL_INCLUDE_DIR)
    _ck_state_get(lib_dir CK_INSTALL_LIB_DIR)
    _ck_state_get(bin_dir CK_INSTALL_BIN_DIR)
    _ck_state_get(debug_dir CK_INSTALL_DEBUG_DIR)

    # A shared module is a runtime artifact on Windows and a library elsewhere, and
    # a static one is an archive: naming the three destinations covers a module
    # whatever it was built as, and the import library of a DLL along with it.
    install(TARGETS ${target}
        EXPORT ${export_set}
        RUNTIME DESTINATION "${bin_dir}"
        LIBRARY DESTINATION "${lib_dir}"
        ARCHIVE DESTINATION "${lib_dir}")

    _ck_install_headers("${directory}/include" "${include_dir}")

    # The generated base is part of the module API, and it is laid out under
    # <Namespace>/<Module>/ like the published headers, so it lands among them.
    _ck_state_get(generated CK_MODULE_${key}_PUBLIC_GENERATED)
    if(generated)
        install(FILES ${generated} DESTINATION "${include_dir}/${namespace}/${name}")
    endif()

    # Renamed on the way, under the name the link option of the module was written
    # against: two modules may each hold a file called '.natvis'.
    _ck_state_get(visualisers CK_MODULE_${key}_VISUALISERS)
    _ck_state_get(visualiser_names CK_MODULE_${key}_VISUALISER_NAMES)
    if(visualisers)
        list(LENGTH visualisers count)
        math(EXPR last "${count} - 1")
        foreach(index RANGE ${last})
            list(GET visualisers ${index} visualiser)
            list(GET visualiser_names ${index} installed_name)
            install(FILES "${visualiser}" DESTINATION "${debug_dir}"
                RENAME "${installed_name}")
        endforeach()
    endif()
endfunction()

# Ships a target that is not a module: a library a module vendored, the resource
# library holding what the project embeds. It has no headers of its own to publish
# and no layout of ours to follow, so what it needs is a place for its binary and a
# seat in the export set, so that the module naming it still resolves.
function(_ck_install_dependency target export_set)
    _ck_state_get(include_dir CK_INSTALL_INCLUDE_DIR)
    _ck_state_get(lib_dir CK_INSTALL_LIB_DIR)
    _ck_state_get(bin_dir CK_INSTALL_BIN_DIR)

    # A library may publish its headers as a file set rather than as a directory it
    # points at, and CMake refuses to export a target whose sets have nowhere to go.
    # They are shipped to the one include root everything else lands in, so a header
    # is reached the same way whichever of the two forms its library chose.
    get_target_property(header_sets ${target} INTERFACE_HEADER_SETS)
    set(file_sets "")
    if(header_sets)
        foreach(header_set IN LISTS header_sets)
            list(APPEND file_sets FILE_SET "${header_set}" DESTINATION "${include_dir}")
        endforeach()
    endif()

    # An older library names its headers through the PUBLIC_HEADER and PRIVATE_HEADER
    # properties instead. Those come from Apple bundles, where such a header travels
    # inside the framework and needs no destination of its own; anywhere else CMake has
    # nowhere to put it and says so, naming the destination it was never given. They go
    # to the same include root, which is where the copy below already puts every header
    # found under the include directories of the library.
    #
    # A relative entry is read against the directory of the install that ships it,
    # and this one is emitted from the root of the project rather than from where the
    # library was declared: left as written, it would name a file that is not there.
    # Anchoring it to the directory the library was read from gives back the file the
    # library meant, and holds for the library's own install too.
    get_target_property(source_dir ${target} SOURCE_DIR)
    set(header_properties "")
    foreach(property PUBLIC_HEADER PRIVATE_HEADER)
        get_target_property(headers ${target} ${property})
        if(NOT headers)
            continue()
        endif()

        set(resolved "")
        foreach(header IN LISTS headers)
            # A generator expression is only answered later, so it is passed through
            # rather than guessed at, the way the include directories below are.
            if(IS_ABSOLUTE "${header}" OR header MATCHES "^\\$<" OR NOT source_dir)
                list(APPEND resolved "${header}")
            else()
                list(APPEND resolved "${source_dir}/${header}")
            endif()
        endforeach()

        set_property(TARGET ${target} PROPERTY ${property} "${resolved}")
        list(APPEND header_properties ${property} DESTINATION "${include_dir}")
    endforeach()

    install(TARGETS ${target}
        EXPORT ${export_set}
        RUNTIME DESTINATION "${bin_dir}"
        LIBRARY DESTINATION "${lib_dir}"
        ARCHIVE DESTINATION "${lib_dir}"
        ${file_sets}
        ${header_properties})

    # Its headers travel too, since a module that publishes a vendored library
    # publishes what that library declares: a consumer compiling the header of the
    # module has to reach it. The include directories it hands its consumers are
    # therefore rewritten the way a module's are, once against the directory it was
    # read from and once against the one it was copied to.
    get_target_property(directories ${target} INTERFACE_INCLUDE_DIRECTORIES)
    if(NOT directories)
        return()
    endif()

    set(rewritten "")
    set(shipped FALSE)

    foreach(directory IN LISTS directories)
        # A library saying it itself which side of its life a directory belongs to:
        # the build one names a real directory, the install one named a prefix of its
        # own and is replaced by ours.
        if(directory MATCHES "^\\$<BUILD_INTERFACE:(.+)>$")
            set(path "${CMAKE_MATCH_1}")
        elseif(directory MATCHES "^\\$<INSTALL_INTERFACE:")
            continue()
        elseif(directory MATCHES "^\\$<")
            # Anything else is a condition only the generator can answer, so it is
            # passed through untouched rather than guessed at.
            list(APPEND rewritten "${directory}")
            continue()
        else()
            set(path "${directory}")
        endif()

        list(APPEND rewritten "$<BUILD_INTERFACE:${path}>")

        if(IS_DIRECTORY "${path}")
            # Headers only, since a library often points at the directory it is
            # written in rather than at one holding what it publishes: the sources
            # beside them are the library's own business and are already compiled
            # into what is being shipped.
            install(DIRECTORY "${path}/" DESTINATION "${include_dir}"
                FILES_MATCHING
                    PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.hh" PATTERN "*.hxx"
                    PATTERN "*.inl" PATTERN "*.inc" PATTERN "*.ipp" PATTERN "*.tcc")
            set(shipped TRUE)
        endif()
    endforeach()

    if(shipped)
        list(APPEND rewritten "$<INSTALL_INTERFACE:${include_dir}>")
    endif()

    set_property(TARGET ${target} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${rewritten})
endfunction()

# Drops from the interface of a shared module what only its own link needed.
#
# A PRIVATE dependency is recorded in the interface of a target as $<LINK_ONLY:...>,
# because a consumer of a static library has to link it too. A shared module has
# already resolved it: the archive is inside the binary, the system library is in its
# dependency list, and nothing a consumer links needs to name either. What that
# leaves behind is a name in the export, which is why it is dropped here: an installed
# module would otherwise ask a consumer for a target it was never given, and the
# X11 or Vulkan a module found for itself would become something whoever links it has
# to find again.
#
# Static modules keep theirs, since there the propagation is what makes them link.
function(_ck_prune_private_interface target)
    get_target_property(type ${target} TYPE)
    if(NOT type STREQUAL "SHARED_LIBRARY")
        return()
    endif()

    get_target_property(libraries ${target} INTERFACE_LINK_LIBRARIES)
    if(NOT libraries)
        return()
    endif()

    set(kept "")
    foreach(library IN LISTS libraries)
        if(library MATCHES "^\\$<LINK_ONLY:")
            continue()
        endif()
        list(APPEND kept "${library}")
    endforeach()

    set_property(TARGET ${target} PROPERTY INTERFACE_LINK_LIBRARIES ${kept})
endfunction()

# The target an interface entry names, when it names one that has to be shipped.
# Empty for everything else: a plain library name the linker resolves on its own, a
# condition the generator answers, a directory that only concerns this build.
function(_ck_interface_dependency out_var entry)
    set(${out_var} "" PARENT_SCOPE)

    set(name "${entry}")
    if(name MATCHES "^\\$<LINK_ONLY:(.+)>$")
        set(name "${CMAKE_MATCH_1}")
    endif()
    # Whatever is scoped to the build tree is not part of what an install ships, and
    # any other condition cannot be resolved at configure time.
    if(name MATCHES "^\\$<")
        return()
    endif()
    if(NOT TARGET ${name})
        return()
    endif()

    _ck_real_target(real "${name}")
    set(${out_var} "${real}" PARENT_SCOPE)
endfunction()

# Package a namespace was found in, empty when nothing said. A Cocktail package says
# it for every namespace it ships, which is how the plugin namespace of a module and
# the package holding it stay two different names: ImGui::ImGuiIntegration is found by
# asking for CocktailEngine, and nothing has to guess that from the target.
function(_ck_namespace_package out_var namespace)
    string(MAKE_C_IDENTIFIER "${namespace}" key)
    _ck_state_get(package CK_PACKAGE_OF_${key})
    set(${out_var} "${package}" PARENT_SCOPE)
endfunction()

# An already built target that stayed in the interface of something being shipped.
#
# The package this project installs asks for it again, so a consumer is handed the
# very target the module was built against. That only works for a namespace the
# framework knows where to find, which every Cocktail package makes sure of: it says
# on being found which namespaces it ships.
#
# Anything else is a target this project found for itself and that its consumers now
# have to find too, with nothing saying where. Reported as a warning and not as an
# error, since the module may well be right and only the package incomplete: what is
# missing is a find_package the consumer makes on its own.
function(_ck_export_foreign_dependency target namespace)
    if(target MATCHES "^([^:]+)::")
        _ck_namespace_package(package "${CMAKE_MATCH_1}")
    else()
        set(package "")
    endif()

    if(package)
        _ck_state_append(CK_INSTALL_DEPENDENCIES "${package}")
        return()
    endif()

    message(WARNING
        "Cocktail Engine: '${target}' is part of what ${namespace} publishes, and it is "
        "an already built target this project cannot ship. Whoever links the installed "
        "${namespace} will have to find it again on its own.\n"
        "Vendoring it PRIVATE keeps it an implementation detail of the module instead.")
endfunction()

# Everything an install has to carry for the modules of this project to resolve:
# the modules themselves, the aggregate a game links, and whatever their interfaces
# still name once the private half has been dropped. Walked transitively, since a
# library a module publishes may publish one of its own.
#
# Each target is paired with the namespace it is shipped under, which is the one of
# the module that reached it: a library vendored by an engine module is exported as
# CocktailEngine::<library>, so it belongs to whoever ships it instead of standing
# for the library itself. Nothing then collides with the same library a consumer
# vendors on its own.
function(_ck_export_targets out_targets out_namespaces)
    _ck_state_get(modules CK_MODULES)

    set(pending "")
    set(pending_namespaces "")

    foreach(module IN LISTS modules)
        _ck_module_key(key "${module}")
        _ck_state_get(target CK_MODULE_${key}_TARGET)
        _ck_state_get(namespace CK_MODULE_${key}_NAMESPACE)
        if(NOT target OR NOT TARGET ${target})
            continue()
        endif()
        _ck_real_target(target "${target}")
        get_target_property(imported ${target} IMPORTED)
        if(imported)
            # Already shipped by whoever built it, and its own package declares it.
            continue()
        endif()
        list(APPEND pending "${target}")
        list(APPEND pending_namespaces "${namespace}")
    endforeach()

    _ck_state_get(generate_aggregate CK_PROJECT_GENERATE_AGGREGATE)
    if(generate_aggregate)
        _ck_project_aggregate(aggregate_target aggregate_alias)
        _ck_state_get(project CK_PROJECT_NAME)
        if(TARGET ${aggregate_target})
            list(APPEND pending "${aggregate_target}")
            list(APPEND pending_namespaces "${project}")
        endif()
    endif()

    set(found "")
    set(found_namespaces "")

    while(pending)
        list(POP_FRONT pending current)
        list(POP_FRONT pending_namespaces namespace)

        if(current IN_LIST found)
            continue()
        endif()
        get_target_property(imported ${current} IMPORTED)
        if(imported)
            # Already built, so it cannot be shipped a second time: it stays a name in
            # the export, and whoever consumes this project has to be handed the same
            # target again. That is what the package does for the packages it was
            # built against, which is the case a module publishing another project's
            # module falls in, and it is why the walk stops here rather than failing.
            _ck_export_foreign_dependency("${current}" "${namespace}")
            continue()
        endif()

        list(APPEND found "${current}")
        list(APPEND found_namespaces "${namespace}")

        get_target_property(libraries ${current} INTERFACE_LINK_LIBRARIES)
        if(NOT libraries)
            continue()
        endif()
        foreach(library IN LISTS libraries)
            _ck_interface_dependency(dependency "${library}")
            if(dependency)
                list(APPEND pending "${dependency}")
                list(APPEND pending_namespaces "${namespace}")
            endif()
        endforeach()
    endwhile()

    set(${out_targets} "${found}" PARENT_SCOPE)
    set(${out_namespaces} "${found_namespaces}" PARENT_SCOPE)
endfunction()

# What the build knows and no target carries: which modules exist, and which
# properties they declare. Replayed by the package as EXTERNAL declarations, so a
# consumer of an installed engine validates a property name against the same schema
# and writes the same configuration a game built here would.
function(_ck_write_module_registry out_file)
    _ck_state_get(project CK_PROJECT_NAME)
    _ck_state_get(modules CK_MODULES)

    set(content "# Modules and properties of ${project}, generated by Cocktail. Do not edit.\n")
    string(APPEND content "#\n")
    string(APPEND content "# Declared EXTERNAL: they are already built, and the targets file beside this\n")
    string(APPEND content "# one is what links them. What is replayed here is what a target cannot hold.\n")

    foreach(module IN LISTS modules)
        _ck_module_key(key "${module}")
        _ck_state_get(target CK_MODULE_${key}_TARGET)
        _ck_state_get(namespace CK_MODULE_${key}_NAMESPACE)
        _ck_state_get(name CK_MODULE_${key}_NAME)

        if(NOT target OR NOT TARGET ${target})
            continue()
        endif()
        get_target_property(imported ${target} IMPORTED)
        if(imported)
            continue()
        endif()

        string(APPEND content "\nck_add_module(${name} EXTERNAL NAMESPACE ${namespace})\n")

        _ck_state_get(keys CK_PROPERTY_KEYS_${module})
        foreach(property_key IN LISTS keys)
            _ck_state_get(property CK_PROPERTY_${property_key}_NAME)
            _ck_state_get(type CK_PROPERTY_${property_key}_TYPE)
            _ck_state_get(values CK_PROPERTY_${property_key}_VALUES)
            _ck_state_get(default CK_PROPERTY_${property_key}_DEFAULT)
            _ck_state_get(description CK_PROPERTY_${property_key}_DESCRIPTION)
            _ck_state_get(required CK_PROPERTY_${property_key}_REQUIRED)

            string(APPEND content "ck_declare_property(\n")
            string(APPEND content "    MODULE ${module}\n")
            string(APPEND content "    NAME ${property}\n")
            string(APPEND content "    TYPE ${type}\n")
            if(values)
                set(quoted "")
                foreach(value IN LISTS values)
                    _ck_quote_argument(value_argument "${value}")
                    list(APPEND quoted "${value_argument}")
                endforeach()
                list(JOIN quoted " " value_arguments)
                string(APPEND content "    VALUES ${value_arguments}\n")
            endif()
            if(NOT default STREQUAL "")
                _ck_quote_argument(argument "${default}")
                string(APPEND content "    DEFAULT ${argument}\n")
            endif()
            if(NOT description STREQUAL "")
                _ck_quote_argument(argument "${description}")
                string(APPEND content "    DESCRIPTION ${argument}\n")
            endif()
            if(required)
                string(APPEND content "    REQUIRED\n")
            endif()
            string(APPEND content ")\n")
        endforeach()
    endforeach()

    file(WRITE "${out_file}" "${content}")
endfunction()

# A value written back as one CMake argument, whatever it holds: a quote, a
# backslash or a ';' that would otherwise be read as a list separator.
function(_ck_quote_argument out_var value)
    string(REPLACE "\\" "\\\\" quoted "${value}")
    string(REPLACE "\"" "\\\"" quoted "${quoted}")
    string(REPLACE ";" "\\;" quoted "${quoted}")
    set(${out_var} "\"${quoted}\"" PARENT_SCOPE)
endfunction()

# The package a consumer finds: the framework itself, the targets of every
# namespace this project ships, and the registry replaying what they cannot carry.
#
# The config file is generated rather than kept as a template beside the framework,
# for the reason the module bases are generated: what it has to include follows from
# what the project turned out to ship, while the facade and its explicit components
# are installed unchanged.
function(_ck_install_package namespaces)
    _ck_state_get(project CK_PROJECT_NAME)
    _ck_state_get(version CK_PROJECT_VERSION)
    _ck_state_get(cmake_dir CK_INSTALL_CMAKE_DIR)
    _ck_state_get(framework_file CK_FRAMEWORK_FILE)
    _ck_state_get(framework_component_dir CK_FRAMEWORK_COMPONENT_DIR)
    _ck_state_get(framework_components CK_FRAMEWORK_COMPONENTS)

    include(CMakePackageConfigHelpers)

    # The project's own namespace first: a plugin module links the modules of the
    # project, so the targets it names have to already be there when it is read.
    list(REMOVE_DUPLICATES namespaces)
    list(REMOVE_ITEM namespaces "${project}")
    list(SORT namespaces)
    list(PREPEND namespaces "${project}")

    set(target_includes "")
    set(namespace_claims "")
    foreach(namespace IN LISTS namespaces)
        string(APPEND target_includes
            "include(\"\${CMAKE_CURRENT_LIST_DIR}/${namespace}Targets.cmake\")\n")
        string(MAKE_C_IDENTIFIER "${namespace}" key)
        string(APPEND namespace_claims
            "set_property(GLOBAL PROPERTY CK_PACKAGE_OF_${key} \"${project}\")\n")
    endforeach()

    # The packages this one was built against and still names in what it publishes.
    # A consumer therefore gets them from finding this one, instead of having to know
    # what it was built on top of.
    _ck_state_get(dependencies CK_INSTALL_DEPENDENCIES)
    set(dependency_calls "")
    if(dependencies)
        list(REMOVE_DUPLICATES dependencies)
        list(SORT dependencies)
        string(APPEND dependency_calls "include(CMakeFindDependencyMacro)\n")
        foreach(dependency IN LISTS dependencies)
            string(APPEND dependency_calls "find_dependency(${dependency})\n")
        endforeach()
        string(APPEND dependency_calls "\n")
    endif()

    set(template "${CMAKE_CURRENT_BINARY_DIR}/${project}Config.cmake.in")
    set(template_content "@PACKAGE_INIT@\n")
    string(APPEND template_content "\n# ${project}, as something already built.\n")
    string(APPEND template_content "#\n")
    string(APPEND template_content
        "# Brings in the framework it was built with, the targets it ships and what the\n")
    string(APPEND template_content
        "# build knew about its modules. A project consuming it therefore declares its own\n")
    string(APPEND template_content
        "# modules and games with the very commands these were declared with:\n")
    string(APPEND template_content "#\n")
    string(APPEND template_content "#   project(MyGame LANGUAGES CXX)\n")
    string(APPEND template_content "#   find_package(${project} REQUIRED)\n")
    string(APPEND template_content "#   ck_project(NAME MyGame AGGREGATE_TARGET)\n")
    string(APPEND template_content "#   ck_add_game(Sandbox)\n")
    string(APPEND template_content "\n")
    string(APPEND template_content
        "# Left alone when the consumer already has the framework, so that a project\n")
    string(APPEND template_content
        "# editing it in place keeps the copy it is working on.\n")
    string(APPEND template_content "if(NOT COMMAND ck_project)\n")
    string(APPEND template_content
        "    include(\"\${CMAKE_CURRENT_LIST_DIR}/${project}Macros.cmake\")\n")
    string(APPEND template_content "endif()\n")
    string(APPEND template_content "\n")
    string(APPEND template_content
        "# Which namespaces this package is where to find, its plugins included. A project\n")
    string(APPEND template_content
        "# publishing one of these modules reads it back to know that its own package has\n")
    string(APPEND template_content
        "# to ask for this one again, which a target name alone would not say: the namespace\n")
    string(APPEND template_content
        "# of a plugin module is not the name of the package shipping it.\n")
    string(APPEND template_content "@CK_PACKAGE_NAMESPACES@")
    string(APPEND template_content "\n@CK_PACKAGE_DEPENDENCIES@")
    string(APPEND template_content "@CK_PACKAGE_TARGET_INCLUDES@")
    string(APPEND template_content
        "\ninclude(\"\${CMAKE_CURRENT_LIST_DIR}/${project}Modules.cmake\")\n")
    string(APPEND template_content "\ncheck_required_components(${project})\n")
    file(WRITE "${template}" "${template_content}")

    set(CK_PACKAGE_NAMESPACES "${namespace_claims}")
    set(CK_PACKAGE_DEPENDENCIES "${dependency_calls}")
    set(CK_PACKAGE_TARGET_INCLUDES "${target_includes}")
    set(config "${CMAKE_CURRENT_BINARY_DIR}/${project}Config.cmake")
    configure_package_config_file("${template}" "${config}"
        INSTALL_DESTINATION "${cmake_dir}")

    install(FILES "${config}" DESTINATION "${cmake_dir}")

    # A version file only when project() was given one: writing a made-up version
    # would have find_package answer a compatibility question nobody asked.
    if(version)
        set(version_file "${CMAKE_CURRENT_BINARY_DIR}/${project}ConfigVersion.cmake")
        write_basic_package_version_file("${version_file}"
            VERSION "${version}" COMPATIBILITY SameMajorVersion)
        install(FILES "${version_file}" DESTINATION "${cmake_dir}")
    endif()

    # The framework as it was when this project was configured, so a consumer builds
    # its modules the way these were built and not the way a newer copy would. The
    # facade is renamed with the package and its component directory follows the
    # same stem, which keeps the development and installed layouts identical.
    install(FILES "${framework_file}" DESTINATION "${cmake_dir}"
        RENAME "${project}Macros.cmake")

    set(framework_component_files "")
    foreach(component IN LISTS framework_components)
        list(APPEND framework_component_files "${framework_component_dir}/${component}")
    endforeach()
    install(FILES ${framework_component_files}
        DESTINATION "${cmake_dir}/${project}Macros")

    set(registry "${CMAKE_CURRENT_BINARY_DIR}/${project}Modules.cmake")
    _ck_write_module_registry("${registry}")
    install(FILES "${registry}" DESTINATION "${cmake_dir}")
endfunction()

# Emits everything an install is made of. Called by ck_finalize, from the directory
# scope ck_project was called from, which is the one the root install script is
# generated for.
function(_ck_install_project)
    _ck_install_enabled(enabled)
    if(NOT enabled)
        return()
    endif()

    _ck_state_get(modules CK_MODULES)

    # Private dependencies go first, since what is dropped here decides what the
    # walk below finds still reachable.
    set(module_targets "")
    foreach(module IN LISTS modules)
        _ck_module_key(key "${module}")
        _ck_state_get(target CK_MODULE_${key}_TARGET)
        if(NOT target OR NOT TARGET ${target})
            continue()
        endif()
        _ck_real_target(target "${target}")
        get_target_property(imported ${target} IMPORTED)
        if(imported)
            continue()
        endif()
        _ck_prune_private_interface(${target})
        list(APPEND module_targets "${target}")
        _ck_state_set(CK_INSTALL_MODULE_OF_${target} "${module}")
    endforeach()

    _ck_export_targets(targets namespaces)
    if(NOT targets)
        return()
    endif()

    list(LENGTH targets count)
    math(EXPR last "${count} - 1")
    foreach(index RANGE ${last})
        list(GET targets ${index} target)
        list(GET namespaces ${index} namespace)
        _ck_export_set(export_set "${namespace}")

        if(target IN_LIST module_targets)
            _ck_state_get(module CK_INSTALL_MODULE_OF_${target})
            _ck_install_module("${module}" "${export_set}")
        else()
            _ck_install_dependency("${target}" "${export_set}")
        endif()
    endforeach()

    _ck_state_get(cmake_dir CK_INSTALL_CMAKE_DIR)
    set(exported_namespaces "${namespaces}")
    list(REMOVE_DUPLICATES exported_namespaces)
    foreach(namespace IN LISTS exported_namespaces)
        _ck_export_set(export_set "${namespace}")
        install(EXPORT ${export_set}
            FILE "${namespace}Targets.cmake"
            NAMESPACE "${namespace}::"
            DESTINATION "${cmake_dir}")
    endforeach()

    _ck_install_package("${exported_namespaces}")

    _ck_state_set(CK_INSTALL_NAMESPACES "${exported_namespaces}")
endfunction()


# =============================================================================
# Embedded resources
#
# Not every file a module reads is an asset a game may swap. The shaders a
# renderer is written against are part of the module in the same way its code is:
# a module missing them does not run at all, and shipping them beside the binary
# would only give someone a chance to remove them. Those are compiled into the
# binary, and reached through the embedded filesystem the engine mounts at
# startup rather than through the disk.
#
# cmrc is what turns a file into a translation unit holding its bytes. Every owner
# declares what it embeds on its own, exactly as it declares its sources, and all
# of it ends up in one resource library for the whole project rather than one per
# owner. What that buys is the C++ side: a cmrc filesystem has to be named by a
# CMRC_DECLARE and handed to the engine by hand, so a library per owner meant that
# every module shipping a file had to remember to write those two lines, and a
# module that forgot compiled its bytes in and could never read them back. One
# library is one declaration, made once by the entry point module, and a module
# has nothing to write at all.
#
# The library is a static one, folded into the program the entry point builds, so
# the bytes live in the executable rather than in each module. That is where the
# filesystem is registered from, and every module reads through the engine, which
# is process-wide: what a module gets out of it does not depend on which binary
# the file was compiled into.
#
# What the C++ side sees of it is a single filesystem to register, and paths
# saying whose file is whose:
#
#   CMRC_DECLARE(Builtin)             written once, by the entry point module
#   cmrc::Builtin::get_filesystem()   what it hands the embedded filesystem driver
#   <owner>/resources/<path>          where a file of that owner is found in it
#
# The published path keeps the layout the files were written in, rooted at the
# resources/ directory of the owner, whether the file was read from there or
# produced into the build tree from something that was. Where the build happens to
# put a generated file therefore never reaches the code asking for it.
#
# What a game carries is therefore everything the project embeds, and not only
# what its own link graph reaches: the library is built once and folded into every
# program, so a plugin module a game does not link still leaves its files in it.
# That is the price of the single declaration, and it is paid in bytes rather than
# in correctness.
# =============================================================================

# Namespace the one resource library publishes itself under, and the name a
# CMRC_DECLARE has to spell. A literal, and deliberately not derived from the
# project: what declares it is the entry point module, which is compiled once and
# cannot name something depending on who is building.
#
# TODO once the engine is exported, a project consuming it will build a resource
# library of its own while the entry point module comes already built. Both cannot
# answer to this namespace, so the driver will need a second one to register, or
# the entry point will have to reach the project library through a symbol the
# project defines.
function(_ck_builtin_namespace out_var)
    set(${out_var} "Builtin" PARENT_SCOPE)
endfunction()

# The resource library itself. Named after the project so that two of them in one
# build tree, which is what an engine and a project sharing it would be, stay
# distinct targets.
function(_ck_builtin_library out_var)
    _ck_state_get(project CK_PROJECT_NAME)
    set(${out_var} "${project}-Builtin" PARENT_SCOPE)
endfunction()

# Directory the embedded files of an owner are published under, and the one its
# generated files are produced into. Every owner gets one, since they all share
# the one filesystem and a path has to say whose it is.
#
# Lowercased for the same reason a module binary is: it is compared as a path, by
# whoever asks the engine for the file.
function(_ck_embedded_root out_var owner)
    string(REGEX REPLACE "^.*::" "" name "${owner}")
    string(TOLOWER "${name}" name)
    set(${out_var} "${name}" PARENT_SCOPE)
endfunction()

# Directory of the owner itself, the module or the game, and not the one of the
# scope that declared it. What is embedded is part of the owner rather than an
# asset of the project, so it is read from where that owner lives: the resources/
# directory a module gets for being a module, the one sitting next to the sources
# of a game.
function(_ck_owner_directory out_var owner kind)
    if(kind STREQUAL "GAME")
        _ck_resource_owner_key(key "${owner}")
        _ck_state_get(directory CK_GAME_${key}_DIRECTORY)
    else()
        _ck_module_key(key "${owner}")
        _ck_state_get(directory CK_MODULE_${key}_DIRECTORY)
    endif()

    set(${out_var} "${directory}" PARENT_SCOPE)
endfunction()

# Adds a file to what the project carries in its programs, under the path it is
# written at in the resources/ directory of its owner. WHENCE roots the published
# path at the directory the file was produced into, so a generated file is
# published as the one it was generated from would have been.
#
# Only registration happens here. The library is created and filled by
# ck_finalize, which is what lets an owner embed a file before the module or the
# game owning it has been declared, and what keeps the whole chain in one
# directory scope: the command producing a generated file and the one turning it
# into a translation unit have to be emitted side by side, or the build system is
# handed a file nothing in its directory knows how to make.
#
# Kept as three lists read by position rather than one list of records, since a
# CMake list has no way of holding a path and a name together.
function(_ck_embed_file owner whence file)
    _ck_state_append(CK_BUILTIN_OWNERS "${owner}")
    _ck_state_append(CK_BUILTIN_WHENCES "${whence}")
    _ck_state_append(CK_BUILTIN_FILES "${file}")
endfunction()

# Target the resource library is folded into: the module holding the entry point,
# hence every program of the project, games and test binaries alike. That is also
# the module writing the one CMRC_DECLARE, so what declares the filesystem and
# what carries it are the same binary.
function(_ck_builtin_host out_var)
    _ck_entry_point_provider(entry_point)

    if(NOT TARGET ${entry_point})
        message(FATAL_ERROR
            "Embedded files need a binary to be compiled into, and '${entry_point}' is "
            "not a target. It is the module holding the entry point, which is what "
            "carries them into every program of the project.")
    endif()

    get_target_property(aliased ${entry_point} ALIASED_TARGET)
    if(aliased)
        set(target "${aliased}")
    else()
        set(target "${entry_point}")
    endif()

    # TODO an installed engine brings its entry point as an imported target, which
    # nothing can be linked into. The project library will then have to be linked
    # into every game and test binary instead.
    get_target_property(imported ${target} IMPORTED)
    if(imported)
        message(FATAL_ERROR
            "Embedded files are folded into '${entry_point}', which this project does not "
            "build: it cannot be linked into. Embedding from a project consuming an "
            "installed engine is not supported yet.")
    endif()

    set(${out_var} "${target}" PARENT_SCOPE)
endfunction()

# Two owners publishing under the same root would write into the same directory of
# the one filesystem, and cmrc keeps the first of two files registered under the
# same path without saying a word. The root is the bare name of the owner, so this
# is two modules of the same name in different namespaces, or a game named like a
# module.
#
# TODO qualifying the root would make it impossible instead of merely caught, at
# the cost of every published path carrying the namespace.
function(_ck_check_builtin_roots)
    _ck_state_get(owners CK_BUILTIN_OWNERS)
    list(REMOVE_DUPLICATES owners)

    set(roots "")
    set(root_owners "")

    foreach(owner IN LISTS owners)
        _ck_embedded_root(root "${owner}")

        list(FIND roots "${root}" index)
        if(NOT index EQUAL -1)
            list(GET root_owners ${index} other)
            message(FATAL_ERROR
                "${owner} and ${other} both embed their files under '${root}/resources', "
                "so one would hide the other. Embedded files are published under the bare "
                "name of their owner, which two of them cannot share.")
        endif()

        list(APPEND roots "${root}")
        list(APPEND root_owners "${owner}")
    endforeach()
endfunction()

# Creates the one resource library of the project and fills it with what every
# owner declared, then folds it into the module holding the entry point. Called by
# ck_finalize, in the directory scope ck_project was called from, which is where
# the commands producing the generated files were emitted too.
function(_ck_embed_builtin_files)
    _ck_state_get(owners CK_BUILTIN_OWNERS)
    if(NOT owners)
        return()
    endif()

    # cmrc is vendored by the Core module, so the command exists from the moment
    # Core has been declared.
    if(NOT COMMAND cmrc_add_resource_library)
        message(FATAL_ERROR
            "Embedding a file needs cmrc, which is not available. It is vendored by the "
            "Core module of the engine, so nothing can be embedded in a project that "
            "does not build or link it.")
    endif()

    _ck_check_builtin_roots()

    _ck_state_get(whences CK_BUILTIN_WHENCES)
    _ck_state_get(files CK_BUILTIN_FILES)
    _ck_builtin_library(library)
    _ck_builtin_namespace(namespace)
    _ck_builtin_host(host)

    cmrc_add_resource_library(${library} NAMESPACE ${namespace})
    # Folded into a static module which is itself folded into an executable, so
    # position independence is not needed today. It is set anyway, for the same
    # reason a vendored library gets it: what a static library ends up inside is
    # not its own business, and a shared one would refuse it otherwise.
    set_target_properties(${library} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        # The real target carries the project in its name to stay unique in a build
        # tree; once installed the namespace of the export set says as much, so what
        # a consumer sees is <Project>::Builtin.
        EXPORT_NAME "Builtin"
        FOLDER "Resources")
    # Built from what the project embeds, so it belongs to the instrumented side of a
    # sanitizer build like the modules and the games it ends up inside.
    _ck_configure_sanitizer(${library})
    # PRIVATE: the bytes are an implementation detail of the program, and nothing
    # links the library to read them. The link still travels, since a static
    # library cannot resolve anything on its own, which is what puts the library
    # after the entry point on the link line.
    target_link_libraries(${host} PRIVATE ${library})

    list(LENGTH owners count)
    math(EXPR last "${count} - 1")

    foreach(index RANGE ${last})
        list(GET owners ${index} owner)
        list(GET whences ${index} whence)
        list(GET files ${index} file)

        _ck_embedded_root(root "${owner}")
        cmrc_add_resources(${library}
            WHENCE "${whence}"
            PREFIX "${root}/resources"
            "${file}")
    endforeach()
endfunction()


# Identifies one compilation. The output name is what an owner cannot repeat, so
# the pair is unique by the very rule ck_add_shader enforces.
function(_ck_shader_key out_var owner output)
    string(MAKE_C_IDENTIFIER "${owner}_${output}" key)
    set(${out_var} "${key}" PARENT_SCOPE)
endfunction()

# Pipeline stage a shader belongs to, read from the extension it is written with.
# The set is glslang's, since -S is what the name ends up as.
function(_ck_shader_stage out_var file)
    set(stages
        "vert;tesc;tese;geom;frag;comp"
        "rgen;rint;rahit;rchit;rmiss;rcall"
        "mesh;task")

    get_filename_component(extension "${file}" LAST_EXT)
    string(REGEX REPLACE "^\\." "" extension "${extension}")
    string(TOLOWER "${extension}" extension)

    if(extension IN_LIST stages)
        set(${out_var} "${extension}" PARENT_SCOPE)
    else()
        set(${out_var} "" PARENT_SCOPE)
    endif()
endfunction()


# Which shader compiler the SPIR-V was produced by, as a file a shader can be
# compiled against.
#
# Naming a target in the DEPENDS of a custom command gets two things at once: the
# ordering, and a file-level dependency on the binary that target produces. The
# second is what makes a shader stale, and it is one relink away from making every
# shader stale: the SPIR-V is what the resource library embeds, so a shader
# recompiled drags the translation unit holding its bytes and the link of every
# program behind it. Fifty variants of a dozen shaders is fifty of those.
#
# A relink is not a new compiler, though, and a build system is told it happened far
# more often than it did. MSBuild tracks the files a link step read, and one of them
# is C:/Windows/System32/tzres.dll, whose date a Windows update is liable to leave in
# the future: every library in the build is then permanently out of date, glslang is
# relinked on every build, and every shader followed.
#
# So the compiler is named where it is run and nowhere else. Ordering survives that:
# a custom command spelling $<TARGET_FILE:> is given a reference to the project
# building it, which is what has the compiler exist before it is called for.
#
# What is left to answer is when the compiler changed, and the stamp is written here,
# while configuring, rather than by a command of its own. A stamp a command produces
# does not help: MSBuild decides that a rule reading a file another rule writes is
# out of date whenever that other rule is going to run, without waiting to see
# whether the file it writes ends up different. One command that has to run is then
# every command after it, which is the cascade the stamp was meant to cut. Written
# while configuring, it belongs to no rule, is never out of date, and moves only when
# a reconfigure finds a compiler that is not the one the SPIR-V was produced by.
function(_ck_shader_compiler_stamp out_var)
    set(stamp "${CMAKE_BINARY_DIR}/shaders/glslang.stamp")
    set(${out_var} "${stamp}" PARENT_SCOPE)

    # glslang states its own version as it configures, from the changelog its release
    # process keeps, and writes it out for the code that includes it to read. That is
    # the compiler's account of itself, and it costs nothing to ask for.
    set(identity "unknown")
    set(build_info "${CMAKE_BINARY_DIR}/include/glslang/build_info.h")
    if(EXISTS "${build_info}")
        file(READ "${build_info}" declarations)
        set(identity "")
        foreach(part IN ITEMS MAJOR MINOR PATCH FLAVOR)
            if(declarations MATCHES "# *define +GLSLANG_VERSION_${part} +([^\r\n]*)")
                string(APPEND identity "${part} ${CMAKE_MATCH_1}\n")
            endif()
        endforeach()
    endif()

    get_filename_component(stamp_dir "${stamp}" DIRECTORY)
    file(MAKE_DIRECTORY "${stamp_dir}")

    # Written only when it differs: the date of this file is the question the shaders
    # ask of it, so writing the same answer again would be answering something else.
    set(recorded "")
    if(EXISTS "${stamp}")
        file(READ "${stamp}" recorded)
    endif()
    if(NOT recorded STREQUAL identity)
        file(WRITE "${stamp}" "${identity}")
    endif()
endfunction()

# Emits the compilation of every recorded shader. Called by ck_finalize, from the
# directory scope the resource library is created in: the SPIR-V is an input of the
# translation unit cmrc generates for it, and a dependency between two commands is
# only seen when both were emitted in the same scope.
function(_ck_compile_shaders)
    _ck_state_get(keys CK_SHADER_KEYS)
    if(NOT keys)
        return()
    endif()

    _ck_shader_compiler_stamp(compiler_stamp)

    foreach(key IN LISTS keys)
        _ck_state_get(owner CK_SHADER_${key}_OWNER)
        _ck_state_get(name CK_SHADER_${key}_NAME)
        _ck_state_get(source_file CK_SHADER_${key}_SOURCE)
        _ck_state_get(output_file CK_SHADER_${key}_BINARY)
        _ck_state_get(stage CK_SHADER_${key}_STAGE)
        _ck_state_get(macros CK_SHADER_${key}_MACRO)

        set(definitions "")
        foreach(macro IN LISTS macros)
            list(APPEND definitions "-D${macro}")
        endforeach()

        # The directory is made now rather than by a command of its own: a process
        # per shader to create a directory that configuring can create once is a
        # third of what compiling a shader costs.
        get_filename_component(output_dir "${output_file}" DIRECTORY)
        file(MAKE_DIRECTORY "${output_dir}")

        # The compiler is named through $<TARGET_FILE:>, which orders the two without
        # making the binary an input: what says the compiler changed is the stamp.
        add_custom_command(
            OUTPUT "${output_file}"
            COMMAND $<TARGET_FILE:glslang-standalone>
                "${source_file}" -o "${output_file}" -V100 -S ${stage} ${definitions}
            DEPENDS "${source_file}" "${compiler_stamp}"
            COMMENT "Compiling ${owner} shader ${name}"
            VERBATIM)
    endforeach()
endfunction()