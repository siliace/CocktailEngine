#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Xlib implementation of a SystemCursor, backed by a glyph of the X11 cursor font
     *
     * X11 ships its shapes as a font rather than as a set of system cursors, so a system
     * cursor here is a glyph of that font instantiated on a given connection.
     */
    class SystemCursor : public Ck::SystemCursor
    {
    public:

        /**
         * \brief Instantiates the cursor font glyph matching a cursor type
         *
         * \param display The shared display connection, which has to outlive this cursor
         * \param type The type of cursor to load
         */
        SystemCursor(::Display* display, SystemCursorType type);

        /**
         * \brief Releases the cursor on the display connection
         */
        ~SystemCursor() override;

        SystemCursorType GetType() const override;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return The X11 cursor identifier, as an untyped pointer
         */
        void* GetSystemHandle() const override;

    private:

        /// Connection the cursor was created on, owned by the service provider.
        ::Display* mDisplay;

        SystemCursorType mType;

        /// X11 cursor identifier, owned by this cursor.
        ::Cursor mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_SYSTEMCURSOR_HPP
