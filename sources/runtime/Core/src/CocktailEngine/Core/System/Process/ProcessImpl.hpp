#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESSIMPL_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESSIMPL_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <CocktailEngine/Core/System/Process/Win32/Process.hpp>
#endif

#ifdef COCKTAIL_OS_GNU_LINUX
#include <CocktailEngine/Core/System/Process/Unix/Process.hpp>
#endif

namespace Ck::Detail
{
#ifdef COCKTAIL_OS_WINDOWS
    using ProcessImpl = Win32::Process;
#endif

#ifdef COCKTAIL_OS_GNU_LINUX
    using ProcessImpl = Unix::Process;
#endif
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESSIMPL_HPP
