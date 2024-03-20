#ifndef COCKTAIL_CORE_IMAGE_IMAGE_HPP
#define COCKTAIL_CORE_IMAGE_IMAGE_HPP

#include <memory>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	struct ImageImportParameters
	{
		/**
		 * \brief
		 */
		enum class Format
		{
			Default,

			/**
			 * \brief
			 */
			Grey,

			/**
			 * \brief
			 */
			GreyAlpha,

			/**
			 * \brief
			 */
			RedGreenBlue,

			/**
			 * \brief
			 */
			RedGreenBlueAlpha,
		};

		/**
		 * \brief 
		 */
		Format ImportFormat = Format::Default;
	};

	/**
	 * \brief 
	 */
	struct ImageExportParameters
	{
		
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Image : public Extends<Image, Object>
	{
	public:

		/**
		 * \brief
		 * \param size
		 * \param pixelFormat
		 * \param pixels
		 */
		Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, const Uint8* pixels);

		/**
		 * \brief 
		 * \param size
		 * \param pixelFormat 
		 * \param pixels 
		 */
		Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, std::unique_ptr<Uint8[]> pixels);

		/**
		 * \brief 
		 * \return 
		 */
		const Extent2D<unsigned int>& GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		const PixelFormat& GetFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		const void* GetPixels() const;

	private:

		Extent2D<unsigned int> mSize;
		PixelFormat mPixelFormat;
		std::unique_ptr<Uint8[]> mPixels;
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGE_HPP
