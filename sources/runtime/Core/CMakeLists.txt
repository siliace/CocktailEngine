ck_declare_thirdparty(
    cyaml
    PUBLIC
    TARGETS cyaml::cyaml
)

ck_declare_thirdparty(
    magic_enum
    PUBLIC
    TARGETS magic_enum::magic_enum
)

ck_declare_thirdparty(
    stb_image
    PRIVATE
    TARGETS stb_image
)

ck_declare_thirdparty(
    stb_image_write
    PRIVATE
    TARGETS stb_image_write
)

ck_declare_thirdparty(
    tinyxml2
    PRIVATE
    TARGETS tinyxml2::tinyxml2
)

if("Win32" IN_LIST CK_PLATFORM_TAGS)
    target_link_libraries(
        CocktailEngine-Core
        PRIVATE winmm ws2_32
    )
endif()

if("Xlib" IN_LIST CK_PLATFORM_TAGS)
    find_package(X11 REQUIRED)
    target_link_libraries(
        CocktailEngine-Core
        PRIVATE dl pthread X11::X11 X11::Xrandr X11::Xcursor
    )
endif()

ck_add_test()
