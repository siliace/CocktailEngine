#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_CURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_CURSOR_HPP

namespace Ck
{
    /**
     * \brief Interface of a mouse cursor shape a Window can display over its client area
     *
     * Base of the two kinds of cursor the engine knows about, the SystemCursor shapes the
     * platform already provides and the ImageCursor shapes an application brings itself.
     * Both are obtained from WindowFactory and handed to Window::SetCursor.
     *
     * Ownership differs between the two: an ImageCursor is owned by whoever created it,
     * whereas a SystemCursor is a non-owning view on a shape cached by the factory.
     * Either way a Window only keeps a non-owning pointer to the cursor it displays, so
     * the cursor has to stay alive as long as it is in use.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland).
     *
     * \see WindowFactory::CreateCursor
     * \see WindowFactory::LoadSystemCursor
     */
    class Cursor
    {
    public:

        /**
         * \brief Virtual destructor
         *
         * Releases the native cursor when the cursor owns one, which is the case of every
         * ImageCursor. Destroying a cursor a window still displays leaves that window
         * pointing at a dead shape.
         */
        virtual ~Cursor() = default;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * What the returned pointer designates is backend-specific: the native cursor
         * handle itself on Win32, and the X11 cursor identifier on X11.
         *
         * \return Non-owning pointer to the native cursor handle, valid as long as the cursor is
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_CURSOR_HPP
