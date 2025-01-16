#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP

#include <Cocktail/Core/Extent3D.hpp>
#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MipMapLevel
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
