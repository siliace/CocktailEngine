#ifndef COCKTAILENGINE_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

#ifdef COCKTAIL_OS_WINDOWS
    #include <CocktailEngine/Core/System/Win32/WindowsServiceProvider.hpp>
#endif

#ifdef COCKTAIL_OS_GNU_LINUX
    #include <CocktailEngine/Core/System/Unix/UnixServiceProvider.hpp>
#endif

namespace Ck::Detail
{
#ifdef COCKTAIL_OS_WINDOWS
    using SystemServiceProviderImpl = Win32::WindowsServiceProvider;
#endif

#ifdef COCKTAIL_OS_GNU_LINUX
    using SystemServiceProviderImpl = Unix::UnixServiceProvider;
#endif
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP