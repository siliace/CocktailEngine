#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP

#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowClass.hpp>
#include <Cocktail/Core/Utility/Cache/CachePool.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class WindowFactory final : public Inherit<WindowFactory, Object, Ck::WindowFactory>
	{
	public:

		/**
		 * \brief 
		 */
		explicit WindowFactory(HINSTANCE instanceHandle);
		
		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param type 
		 * \return 
		 */
		Ref<Ck::SystemCursor> LoadSystemCursor(SystemCursorType type) override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Ref<Ck::Window>>& OnWindowCreated() override;

	private:

		HINSTANCE mInstanceHandle;
		WindowClass mWindowClass;
		CachePool<SystemCursorType, Ref<Ck::SystemCursor>> mSystemCursors;
		Signal<Ref<Ck::Window>> mOnWindowCreated;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP