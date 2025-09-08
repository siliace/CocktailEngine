#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP

#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class WindowClass
	{
	public:

		/**
		 * \brief 
		 * \param instanceHandle
		 * \param windowProc 
		 * \param className 
		 */
		WindowClass(HINSTANCE instanceHandle, WNDPROC windowProc, const TextChar* className);

		/**
		 * \brief 
		 */
		~WindowClass();

		/**
		 * \brief 
		 * \return 
		 */
		ATOM GetHandle() const;
		
	private:

		HINSTANCE mInstanceHandle;
		ATOM mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP
