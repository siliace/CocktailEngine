#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP

#include <Cocktail/Core/Cocktail.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <Cocktail/Core/System/Win32/Windows.hpp>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETHEADER_HPP
