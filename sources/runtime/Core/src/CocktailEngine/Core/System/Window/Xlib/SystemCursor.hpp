#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>

namespace Ck::Detail::Xlib
{
	/**
	 * \brief 
	 */
	class SystemCursor : public Ck::SystemCursor
	{
	public:

		/**
		 * \brief 
		 * \param type 
		 */
		SystemCursor(::Display* display, SystemCursorType type);

		/**
		 * \brief 
		 */		
		~SystemCursor() override;
		
		/**
		 * \brief 
		 * \return 
		 */
		SystemCursorType GetType() const override;
		
		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		::Display* mDisplay;
		SystemCursorType mType;
		int mHandle;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
