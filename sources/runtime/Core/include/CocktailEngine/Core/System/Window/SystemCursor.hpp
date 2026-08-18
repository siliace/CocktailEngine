#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Window/Cursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursorType.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
