#ifndef COCKTAIL_CORE_SYSTEM_WIN32_WINDOWS_HPP
#define COCKTAIL_CORE_SYSTEM_WIN32_WINDOWS_HPP

#include <Windows.h>
#include <Windowsx.h>

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
