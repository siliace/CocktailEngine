ck_add_module(
    Core
    STATIC
    MODULE_CLASS OFF
    DEPENDS
        CocktailEngine::Core
)

ck_add_module(
    FileSystemDriver
    MODULE_CLASS OFF
    DEPENDS
        CocktailEngine::Core
        Ftp::Core
)

ck_add_module(
    FileSystemServer
    MODULE_CLASS OFF
    DEPENDS
        CocktailEngine::Core
        Ftp::Core
)