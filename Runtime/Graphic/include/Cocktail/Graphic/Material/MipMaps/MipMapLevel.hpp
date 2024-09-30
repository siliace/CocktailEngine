#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP

#include <memory>

#include <Cocktail/Core/Extent3D.hpp>
#include <Cocktail/Core/Image/PixelFormat.hpp>

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
		 * \brief 
		 * \param size 
		 * \param pixelFormat 
		 */
		MipMapLevel(const Extent3D<unsigned int>& size, const PixelFormat& pixelFormat);

		/**
		 * \brief
		 * \param size
		 * \param pixelFormat
		 * \param pixels
		 */
		MipMapLevel(const Extent3D<unsigned int>& size, const PixelFormat& pixelFormat, const void* pixels);

		/**
		 * \brief 
		 * \param pixels 
		 */
		void CopyPixels(const void* pixels) const;

		/**
		 * \brief 
		 * \return 
		 */
		const Extent3D<unsigned int>& GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		const PixelFormat& GetPixelFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		const void* GetPixels() const;

	private:

		Extent3D<unsigned int> mSize;
		PixelFormat mPixelFormat;
		std::unique_ptr<unsigned char[]> mPixels;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPLEVEL_HPP
