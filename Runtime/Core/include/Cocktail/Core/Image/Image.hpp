#ifndef COCKTAIL_CORE_IMAGE_IMAGE_HPP
#define COCKTAIL_CORE_IMAGE_IMAGE_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

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
			/**
			 * \brief 
			 */
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
		std::string Format;
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Image
	{
	public:

		/**
		 * \brief 
		 * \param size
		 * \param pixelFormat 
		 * \param pixels 
		 */
		Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, ByteArray pixels);

		/**
		 * \brief
		 * \param size
		 * \param pixelFormat
		 * \param pixels
		 */
		Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, const void* pixels);

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
		const ByteArray& GetPixels() const;

	private:

		Extent2D<unsigned int> mSize;
		PixelFormat mPixelFormat;
		ByteArray mPixels;
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGE_HPP
