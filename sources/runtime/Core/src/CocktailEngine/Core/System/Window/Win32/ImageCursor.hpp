#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP

#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/ImageCursor.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
