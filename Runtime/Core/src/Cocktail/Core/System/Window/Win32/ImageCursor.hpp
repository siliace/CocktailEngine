#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP

#include <Cocktail/Core/Image/Image.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Window/ImageCursor.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class ImageCursor : public Ck::ImageCursor
	{
	public:

		/**
		 * \brief 
		 * \param image 
		 * \param hotspot 
		 */
		ImageCursor(const Image& image, const Extent2D<unsigned int>& hotspot);

		/**
		 * \brief 
		 */
		~ImageCursor() override;
		
		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned int> GetHotSpot() const override;
		
		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		Extent2D<unsigned int> mHotspot;
		HCURSOR mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
