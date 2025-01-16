#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Window/Cursor.hpp>
#include <Cocktail/Core/System/Window/SystemCursor.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class SystemCursor : public Inherit<SystemCursor, Object, Ck::SystemCursor>
	{
	public:

		/**
		 * \brief 
		 * \param type 
		 */
		explicit SystemCursor(SystemCursorType type);
		
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

		SystemCursorType mType;
		HCURSOR mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
