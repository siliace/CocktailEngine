#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP

#include <CocktailEngine/Core/Extent2D.hpp>
#include <CocktailEngine/Core/System/Window/Cursor.hpp>

namespace Ck
{
	class ImageCursor : public Cursor
	{
	public:

		/**
		 * \brief
		 * \return
		 */
		virtual Extent2D<unsigned int> GetHotSpot() const = 0;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
