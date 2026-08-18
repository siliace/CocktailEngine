#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP

#include <CocktailEngine/Core/System/Window/Xlib/Window.hpp>
#include <CocktailEngine/Core/System/Window/WindowFactory.hpp>
#include <CocktailEngine/Core/Utility/EnumMap.hpp>

namespace Ck::Detail::Xlib
{
    class WindowFactory final : public Ck::WindowFactory
    {
    public:

		explicit WindowFactory(::Display* display);

		UniquePtr<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

		UniquePtr<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

		Ck::SystemCursor* LoadSystemCursor(SystemCursorType type) override;

		bool IsSystemCursorSupported(SystemCursorType systemCursorType) const override;

		::Display* GetDisplay() const;

	private:

		::Display* mDisplay;
		EnumMap<SystemCursorType, UniquePtr<Ck::SystemCursor>> mSystemCursors;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
