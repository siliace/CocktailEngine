#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP

#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Scoped registration of a Win32 window class, the template every window is created from
     *
     * Win32 requires a class to be registered with the system before any window can be
     * created from it, and it is the class that carries the window procedure. Wrapping the
     * registration in an object ties its lifetime to the factory owning it, so the class is
     * unregistered when the factory goes away.
     *
     * Registration is per module and per class name, so a single instance is meant to be
     * shared by every window of the process.
     */
    class WindowClass
    {
    public:

        /**
         * \brief Registers the window class with the system
         *
         * \param instanceHandle Handle of the module the class is registered for
         * \param windowProc The window procedure every window of this class dispatches its messages to
         * \param className Name identifying the class, which has to be unique within the module
         */
        WindowClass(HINSTANCE instanceHandle, WNDPROC windowProc, const TextChar* className);

        /**
         * \brief Unregisters the window class
         *
         * Windows refuses to unregister a class that still has windows, so this silently
         * does nothing when one of them outlived the class.
         */
        ~WindowClass();

        /**
         * \brief Retrieves the atom identifying the registered class
         *
         * \return The class atom, to be passed as the class name when creating a window
         */
        ATOM GetHandle() const;

    private:

        HINSTANCE mInstanceHandle;
        ATOM mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWCLASS_HPP
