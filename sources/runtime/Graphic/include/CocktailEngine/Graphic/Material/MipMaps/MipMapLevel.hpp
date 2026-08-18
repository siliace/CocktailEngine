#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP

#include <CocktailEngine/Core/Extent3D.hpp>
#include <CocktailEngine/Renderer/PixelFormat.hpp>
#include <CocktailEngine/Core/Utility/ByteArray.hpp>

#include <CocktailEngine/Graphic/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_GRAPHIC_API MipMapLevel
	{
	public:

		/**
		 * \brief Copy pixels to the level
		 * \param pixels Pixels to copy
		 */
		void CopyPixels(const void* pixels);

		/**
		 * \brief Copy pixels to a sub region of the level
		 * \param offset The offset where copying pixels
		 * \param size The size of pixels to copy
		 * \param pixels Pixels to copy
		 */
		void CopyPixels(Extent3D<unsigned int> offset, Extent3D<unsigned int> size, const void* pixels);

		/**
		 * \brief 
		 * \return
		 */
		bool IsComplete() const;

		/**
		 * \brief
		 * \return 
		 */
		Extent3D<unsigned int> GetSize() const;

		/**
		 * \brief Get the pixel format used by the level
		 * \return The pixel format
		 */
		PixelFormat GetPixelFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		ByteArrayView GetPixels() const;

	private:

		friend class MipMaps;

		/**
		 * \brief 
		 * \param size 
		 * \param allocationSize 
		 */
		MipMapLevel(Extent3D<unsigned int> size, const PixelFormat& pixelFormat, std::size_t allocationSize);

		Extent3D<unsigned int> mSize;
		PixelFormat mPixelFormat;
		ByteArray mPixels;
		bool mComplete;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP
