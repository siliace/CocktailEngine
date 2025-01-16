#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Image/Image.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>
#include <Cocktail/Core/System/Window/ImageCursor.hpp>

namespace Ck::Detail::Xlib
{
	/**
	 * \brief 
	 */
	class ImageCursor : public Inherit<ImageCursor, Object, Ck::ImageCursor>
	{
	public:

		/**
		 * \brief 
		 * \param createInfo 
		 */
		ImageCursor(::Display* display, const Image& image, const Extent2D<unsigned int>& hotspot);

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

		::Display* mDisplay;
		Extent2D<unsigned int> mHotSpot;
		int mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
