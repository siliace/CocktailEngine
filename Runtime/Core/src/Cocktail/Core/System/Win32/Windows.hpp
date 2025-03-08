#ifndef COCKTAIL_CORE_SYSTEM_WIN32_WINDOWS_HPP
#define COCKTAIL_CORE_SYSTEM_WIN32_WINDOWS_HPP

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <mmsystem.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#ifdef CreateWindow
	#undef CreateWindow
#endif

#ifdef CreateDirectory
	#undef CreateDirectory
#endif

#ifdef CreateFile
	#undef CreateFile
#endif

#ifdef LoadLibrary
	#undef LoadLibrary
#endif

#ifdef IsMinimized
	#undef IsMinimized
#endif

#ifdef IsMaximized
	#undef IsMaximized
#endif

#endif // COCKTAIL_CORE_SYSTEM_WIN32_WINDOWS_HPP
