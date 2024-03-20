#ifndef COCKTAIL_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP
#define COCKTAIL_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP

#ifdef COCKTAIL_OS_WINDOWS
#include <Cocktail/Core/System/Win32/WindowsServiceProvider.hpp>
#endif

namespace Ck::Detail
{
#ifdef COCKTAIL_OS_WINDOWS
    using SystemServiceProviderImpl = Win32::WindowsServiceProvider;
#else
    #error Lack of implementation: SystemServiceProviderImpl;
#endif
}

#endif // COCKTAIL_CORE_SYSTEM_WIN32_SYSTEMSERVICEPROVICERIMPL_HPP