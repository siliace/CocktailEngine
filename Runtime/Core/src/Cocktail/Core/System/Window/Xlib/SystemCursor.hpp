#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP

#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>
#include <Cocktail/Core/System/Window/SystemCursor.hpp>

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

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
