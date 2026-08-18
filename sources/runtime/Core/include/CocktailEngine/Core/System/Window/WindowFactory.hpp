#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP

#include <CocktailEngine/Core/System/Window/ImageCursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursorType.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>
#include <CocktailEngine/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck
{
    /**
     * \brief Base interface exposing Cursor, Window and WindowEventQueue creation
     *
     * The entry point of the whole module: the only way to obtain a Window or a Cursor,
     * and the one place where the windowing backend of the platform is selected. The
     * service is registered as a singleton by the platform service provider, so it is
     * resolved from the Application rather than instantiated.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland). Which system cursors exist is the visible part of that
     * variance, hence IsSystemCursorSupported.
     *
     * Ownership is not uniform across this interface. Windows and image cursors are
     * handed to the caller, whereas system cursors are cached by the factory and returned
     * as non-owning views: they live as long as the factory and must not be deleted.
     */
    class WindowFactory
    {
    public:

        /**
         * \brief Destructor
         *
         * Releases the cached system cursors, which outlives neither the factory nor, by
         * extension, the windows still displaying one of them.
         */
        virtual ~WindowFactory() = default;

        /**
         * \brief Create a new Cursor
         *
         * Converts the image into a native cursor once, so the image can be released as
         * soon as this function returns. The image is expected to carry the
         * ImageRawFormat::Type::RedGreenBlueAlpha8 raw format, the alpha channel being
         * what shapes the cursor.
         *
         * \param image The image to use as the cursor shape
         * \param hotspot The pixel of the image designating the pointed position, from its top-left corner
         *
         * \return The created Cursor, owned by the caller, which has to outlive every window displaying it
         */
        virtual UniquePtr<ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) = 0;

        /**
         * \brief Create a new Window
         *
         * The window is created on, and belongs to, the calling thread: that is the thread
         * that has to call Window::PollEvents afterwards. Its keyboard and mouse events
         * are also forwarded to the global KeyboardService and MouseService, which is what
         * makes the Keyboard and Mouse facades report anything.
         *
         * \param createInfo The structure describing the Window to create
         *
         * \return The created Window, owned by the caller
         */
        virtual UniquePtr<Window> CreateWindow(const WindowCreateInfo& createInfo) = 0;

        /**
         * \brief Load a system's Cursor
         *
         * Cursors are created once and cached, so loading the same type twice returns the
         * same instance and loading one is cheap enough to do on every cursor change.
         *
         * \param type The type of Cursor to load
         *
         * \return Non-owning pointer to the loaded cursor, valid as long as this factory is,
         *         or nullptr if the type is not supported on this platform
         */
        virtual SystemCursor* LoadSystemCursor(SystemCursorType type) = 0;

        /**
         * \brief Tell whether a SystemCursorType is supported
         *
         * Not every platform provides every shape. Code that cares about looking right
         * rather than looking native is expected to test the type first and fall back on a
         * supported one, SystemCursorType::Arrow being available everywhere.
         *
         * \param systemCursorType The cursor type to test
         *
         * \return True if supported, false otherwise
         */
        virtual bool IsSystemCursorSupported(SystemCursorType systemCursorType) const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
