#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP

#include <Cocktail/Core/System/Window/Xlib/Window.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/Cache/CachePool.hpp>

namespace Ck::Detail::Xlib
{
    class WindowFactory final : public Inherit<WindowFactory, Object, Ck::WindowFactory>
    {
    public:

		explicit WindowFactory(::Display* display);

		Ref<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

		Ref<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

		Ref<Ck::SystemCursor> LoadSystemCursor(SystemCursorType type) override;

		Signal<Ref<Ck::Window>>& OnWindowCreated() override;
		
		::Display* GetDisplay() const;

	private:

		::Display* mDisplay;
		Signal<Ref<Ck::Window>> mOnWindowCreated;
		CachePool<SystemCursorType, Ref<Ck::SystemCursor>> mSystemCursors;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
