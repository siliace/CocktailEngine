#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP
