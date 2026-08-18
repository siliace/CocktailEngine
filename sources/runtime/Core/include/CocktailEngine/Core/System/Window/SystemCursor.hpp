#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Window/Cursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursorType.hpp>

namespace Ck
{
    /**
     * \brief Interface of a Cursor whose shape is one of those the platform already provides
     *
     * Using these rather than an ImageCursor is what makes an application look native,
     * since the user gets the very shapes the rest of their desktop uses. They are
     * obtained from WindowFactory::LoadSystemCursor, which caches them for the lifetime of
     * the factory and hands out non-owning views, so they are never deleted by the caller.
     *
     * \see WindowFactory::LoadSystemCursor
     */
    class SystemCursor : public Cursor
    {
    public:

        /**
         * \brief Retrieves the type of system shape this cursor carries
         *
         * \return The system cursor type this cursor was loaded for
         */
        virtual SystemCursorType GetType() const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_SYSTEMCURSOR_HPP
