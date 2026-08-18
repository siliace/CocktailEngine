#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP

#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Win32 implementation of a SystemCursor, backed by one of the shared shapes of the system
     *
     * The shapes are owned by the system and shared by every application, which is why
     * this class has no destructor to speak of.
     */
    class SystemCursor : public Ck::SystemCursor
    {
    public:

        /**
         * \brief Loads the system shape matching a cursor type
         *
         * \param type The type of cursor to load
         */
        explicit SystemCursor(SystemCursorType type);

        SystemCursorType GetType() const override;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return The HCURSOR of this cursor, as an untyped pointer
         */
        void* GetSystemHandle() const override;

    private:

        SystemCursorType mType;

        /// Shape shared with the rest of the system, which this cursor does not own.
        HCURSOR mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_SYSTEMCURSOR_HPP
