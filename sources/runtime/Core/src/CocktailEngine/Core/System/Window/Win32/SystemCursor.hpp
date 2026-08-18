#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>

namespace Ck::Detail::Win32
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

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
