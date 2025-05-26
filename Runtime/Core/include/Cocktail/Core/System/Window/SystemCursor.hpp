#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP

#include <Cocktail/Core/System/Window/Cursor.hpp>
#include <Cocktail/Core/System/Window/SystemCursorType.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class SystemCursor : public Cursor
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual SystemCursorType GetType() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
