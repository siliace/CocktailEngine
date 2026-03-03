#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP

#include <Cocktail/Core/System/Window/Xlib/Window.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

namespace Ck::Detail::Xlib
{
    class WindowFactory final : public Ck::WindowFactory
    {
    public:

		explicit WindowFactory(::Display* display);

		std::shared_ptr<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

		UniquePtr<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

		std::shared_ptr<Ck::SystemCursor> LoadSystemCursor(SystemCursorType type) override;

		bool IsSystemCursorSupported(SystemCursorType systemCursorType) const override;

		::Display* GetDisplay() const;

	private:

		::Display* mDisplay;
		EnumMap<SystemCursorType, std::shared_ptr<Ck::SystemCursor>> mSystemCursors;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
