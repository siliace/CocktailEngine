#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP

#include <CocktailEngine/Core/System/Window/WindowFactory.hpp>
#include <CocktailEngine/Core/System/Window/Xlib/Window.hpp>
#include <CocktailEngine/Core/Utility/EnumMap.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Xlib implementation of the WindowFactory, registered as a singleton by the Xlib service provider
     *
     * Hands the shared display connection to everything it creates and owns the cache of
     * system cursors, which are display-bound and therefore outlive the windows
     * displaying them. The connection itself belongs to the service provider.
     */
    class WindowFactory final : public Ck::WindowFactory
    {
    public:

        /**
         * \brief Loads the system cursors supported on X11 for the given connection
         *
         * \param display The shared display connection, which has to outlive this factory
         */
        explicit WindowFactory(::Display* display);

        UniquePtr<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

        UniquePtr<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

        Ck::SystemCursor* LoadSystemCursor(SystemCursorType type) override;

        /**
         * \brief Tell whether a SystemCursorType is supported
         *
         * The X11 cursor font has no diagonal double arrow, so the two diagonal resize
         * shapes are the only ones this backend cannot serve.
         *
         * \param systemCursorType The cursor type to test
         *
         * \return True if supported, false otherwise
         */
        bool IsSystemCursorSupported(SystemCursorType systemCursorType) const override;

        /**
         * \brief Retrieves the display connection this factory creates its windows on
         *
         * \return Non-owning pointer to the shared display connection
         */
        ::Display* GetDisplay() const;

    private:

        /// Connection shared by every window of the process, owned by the service provider.
        ::Display* mDisplay;

        /// System cursors loaded once and handed out as non-owning views by LoadSystemCursor.
        EnumMap<SystemCursorType, UniquePtr<Ck::SystemCursor>> mSystemCursors;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOWFACTORY_HPP
