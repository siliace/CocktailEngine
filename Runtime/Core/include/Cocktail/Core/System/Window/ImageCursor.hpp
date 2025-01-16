#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP

#include <Cocktail/Core/Extent2D.hpp>
#include <Cocktail/Core/System/Window/Cursor.hpp>

namespace Ck
{
	class ImageCursor : public Implements<ImageCursor, Cursor>
	{
	public:

		/**
		 * \brief
		 * \return
		 */
		virtual Extent2D<unsigned int> GetHotSpot() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
