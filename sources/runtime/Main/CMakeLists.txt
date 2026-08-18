ck_declare_thirdparty(
    cmrc
    PRIVATE
    TARGETS cmrc::base
)

if("Win32" IN_LIST CK_PLATFORM_TAGS)
    # DbgHelp is what walks a stack and writes a minidump, which is the crash handler of
    # this platform. There is no equivalent to link on the other one: the unwinder behind
    # backtrace ships with the C library.
    target_link_libraries(
        CocktailEngine-Main
        PRIVATE dbghelp
    )
endif()

if("Unix" IN_LIST CK_PLATFORM_TAGS)
    # A crash report is worth reading only if its frames have names, and backtrace reads
    # them from the dynamic symbol table. Without this an executable exports nothing, so
    # every frame in the program itself comes out as a bare offset.
    #
    # INTERFACE: this module is static and folded into each program, and it is the program
    # that has to be linked with it.
    target_link_options(
        CocktailEngine-Main
        INTERFACE -rdynamic
    )
endif()
